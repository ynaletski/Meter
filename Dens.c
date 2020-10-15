#include "ALL.h"

//
//
//

char sw_ver[20]="N1 29 June 2020";

int  MMI_inp_err=0;
int  MMI_out_err=0;
int  ZeroPage=2;
int  EmptPage=2;

unsigned long ComBaud[5]={9600,9600,9600,9600,115200};
int Com_lgth[5]={8,8,8,8,8};
int Com_parity[5]={0,0,0,0,0};//0-(None);1-(Even);2-(Odd);3-(MARK, always 1); 4-(SPACE,always 0)

int Com_stop[5]={1,1,1,1,1};

int  ComProt[5]={0   ,0,1,0,0};

long int baudrate_val[]={1200,1200,1200,1200,2400,4800,9600,19200,38400,57600,115200};

//------------------------
int f_get_nb( long int baud)
{
int i;
 for(i=3;i<11;i++)
  if(baudrate_val[i]== baud)
    return i;
 return -1;
}
//------------------------
int getch_host()
{
while(!IsCom(ComPortHost));
 return(ReadCom(ComPortHost));
}
//-------------------
int puts_host(char *str)
{
int i;
 for(i=0;str[i];i++)
 putch(str[i]);
 return i;
}
//-------------------
/* ================================================== */
int f_cycle()
{   // основной цикл

    if( WDOG[0].status)  f_WDOG(0);

    ServiceCOM(1);
    ServiceCOM(2);
    ServiceCOM(3);

    #if defined(ICP_7017C)
    if(I7017C[0].status)  f_7017C(0);
    #endif

    if(UFG[0].status)  f_UFG(0);
    else 
    {
      s_UFG.dens=-2;
      s_UFG.temp=-2;
      s_UFG.vol=-2;
    }
    
    f_one_sec();

    if(ff_serv != NULL) (*ff_serv)(); // сканирование магистрали,

    if(f_menu_MMI()) return -1;
    
    if(MMI.status)
    {
        f_MMI();
    }

    if(f_host()) return -1;

    return 0;
}
/* ================================================== */

/* ---- main ------------------------------------------------------------- */

int ttt1,ttt2,ttt3,ttt4;
int itt1,itt2,itt3,itt4;
long int rltt1,rltt2,rltt3,rltt4;
long int ltt1,ltt2,ltt3,ltt4;
int  flag_prn=0;

unsigned long ttt1_l,ttt2_l,ttt3_l;

int flag_H=0;

void main(void)
{
    double dtmp1,dtmp2,dtmp3,dtmp4;
    float ftmp,ftmp1,ftmp2,ftmp3,ftmp4;
    int iitmp1,iitmp2,iitmp3;
    int i,j;

    char cmd[40];
    int key;
    int iRet;
    int year,month,day,hour,min,sec;

    int itmp;
    InitLib();
    tzset();
    //daylight=0;
    //timezone=0;

    f_queue_init();
    f_ee_num_init();
    if(f_chk_EEE_CRC()<0)
    {
      m_n_ee:
      InstallCom(ComPortHost,ComBaud[4],8,0,1);
      printf("CRC Error.Default loaded\n");
      sw_mmi=99;
    }
    else
    {
      f_rd_eee();
    }

    ComBaud[ComPortHost]=115200L;
    InstallCom(ComPortHost,ComBaud[ComPortHost],Com_lgth[ComPortHost],Com_parity[ComPortHost],Com_stop[ComPortHost]);

    f_clr_scr_MMI();

    if(sw_mmi==0 )
    {
        printf("\nFacom Ver.%s",sw_ver);
        f_prn_begin();
    }
    else
    {
      f_prn_CRC_error();
    }
    
    SetDisplayPage(ZeroPage);
    
    for(i=1;i<4;i++)
    {
    if(f_get_nb(ComBaud[i]) <0)  ComBaud[i]=9600L;
    InstallCom(i,ComBaud[i],Com_lgth[i],Com_parity[i],Com_stop[i]);
    }

    printf("\n\r>");

    one_sec_time=TimeStamp;

    while(1)
    {
        if(f_cycle()<0) break;
    }
    printf("\n\rExit\n\r");

    RestoreCom(1);
    RestoreCom(2);
    RestoreCom(3);
    while(!IsCom4OutBufEmpty()) ;  /* Wait all message sent out of COM4 */
    RestoreCom(4);
}
/* ================================================== */


/* ================================================== */
int last_out[5]={-1,-1,-1,-1,-1};
long int scom_tim[5]={1,1,1,1,1};
long int Tm_snd[5]= {0,0,0,0,0};

void ServiceCOM ( int ii)
/*
  Функция принимает ответ из SLAVE контроллера
  Функция принимает ответ на команду посланную в  COM ii.
  После завершения приема команды, принятая команда обрабатывается
  потребителем, пославшим команду в Slave
*/
{
int i,Addr_rcv,itmp;
struct COM_rqst *request;
struct s_icp_dev *ICPl_DEV;
int  tmp;

 if(scom_tim[ii]!=0)
  if(  f_timer(time_rcv[ii],scom_tim[ii]) == 0)
  {
   if(IsCom(ii)!=0)
   {
     while(IsCom(ii)!=0)
     {
      tmp= ReadCom(ii);

    if(flag_Deb)
      printf("\n%02X",tmp&0xff);
      time_rcv[ii]= TimeStamp;
     }
   }
   return;
  }

  if(COM_que[ii].busy_ptr != COM_que[ii].empt_ptr)
  {
    if(last_out[ii] != COM_que[ii].empt_ptr)
    {
      request=COM_que[ii].request[COM_que[ii].empt_ptr];
      if(request==NULL)
         goto m1;

      last_out[ii]=COM_que[ii].empt_ptr;

      // подготовка к обслуживанию запроса и вывод в порт

      ff_gets_com[ii]=request->gets_com;
      ff_answ_com[ii]=request->answ_com;
      ff_answ_flt[ii]=request->answ_flt;
      ToutAns[ii]=request->timeout;
      time_snd[ii]= TimeStamp;
      Tm_snd[ii]= TimeStamp;

      ICPl_DEV=request->ICP_dd;
      ICPl_DEV->time_stamp_snd=TimeStamp-request->time_stamp;

      // вывод в порт
//  ltt1=TimeStamp;
//  tim1();

#if defined(I7188D)
  Set485DirToTransmit(ii); //qqq
#endif

   ToComBufn(ii,request->Cmd,request->cmd_lgth);
/*
   if(ii == 3)
   {  printf("\n\rOUT_C3=");
      for(i=0;i<request->cmd_lgth;i++)
      putch(request->Cmd[i]);
   }
*/
#if defined(I7188D)
  WaitTransmitOver(ii);
  Set485DirToReceive(ii);
#endif

//  tim2();
//  ltt2=TimeStamp;

//   itt1=ttt2-ttt1-10;  // 10 следует отнять - это время присутствует
//   if(itt1 < 0) itt1+=10000;
//   rltt1=ltt2-ltt1;

//   if(flag_prn)
//   {
//     flag_prn=0;
//     printf("\n %d bytes send, time=%ld ms, %.1f mks",request->cmd_lgth,rltt1,(float)(itt1)/10.);
//   }
      request->status=2;
    }
  }

m1:
  if( (*ff_gets_com[ii])(ii) )
 /* Ввод ответа на команду. Указатель на функцию ввода
    устанавливает функция, инициировавшая передачу команды
 */
  {
     time_rcv[ii]= TimeStamp;
     request=COM_que[ii].request[COM_que[ii].empt_ptr];
     ICPl_DEV=request->ICP_dd;
     ICPl_DEV->time_stamp_rcv=TimeStamp - request->time_stamp;

   /* в cb_COM[ii][] находится команда длиной  n_bcom[ii]  */

   /* Обработка команды. Указатель на функцию обработки устанавливает
      функция, инициирующая передачу
   */

     ff_gets_com[ii]=f_empty_gets;
     (*ff_answ_com[ii])(ii);

     ff_answ_com[ii]=f_empty;
     n_bcom[ii]=0;
  }
}
/* ================================================== */
int f_timer(unsigned long int time_stamp,unsigned long int delay)
{
// если время с момента записи временного штампа превышает
 // delay мс - возвращает 1, иначе 0.
// if((TimeStamp-time_stamp) > delay) return 1;
// else return 0;
 if((TimeStamp-time_stamp - delay) & 0x80000000 ) return 0;
 else return 1;
}
/* ================================================== */

unsigned long int time_snd[5]={0,0,0,0,0};
unsigned long int time_rcv[5]={0,0,0,0,0};

char cb_COM[5][n_bufcom];
int ghj=0;
int n_bcom[5]={0,0,0,0,0};

void (*ff_serv)() = NULL;

int  (*ff_gets_com[5])(int ii) = {f_empty_gets,f_empty_gets,f_empty_gets,f_empty_gets,f_empty_gets};
void (*ff_answ_com[5])(int ii) = {f_empty,f_empty,f_empty,f_empty,f_empty};
void (*ff_answ_flt[5])(int ii) = {f_empty,f_empty,f_empty,f_empty,f_empty};
int i_empty;
void f_empty(int ii)
{
 i_empty=ii;
}
/* ---------------------------------------- */
unsigned long int one_sec_time=0;
#define nr_year   0
#define nr_month  1
#define nr_day    2
#define nr_hour   3
#define nr_min    4
#define nr_sec    5
//------------------------------
struct arch_str_t // 24 bytes
{
unsigned  char sec;
unsigned  char min;
unsigned  char hour;
unsigned  char day  ;
unsigned  char month;
unsigned  char year ;
};
//------------------------------

void f_one_sec()
{
union  { float f; char c[4]; } o;
  int win_sum;
  TIME_DATE t;

  if( f_timer(one_sec_time,1000L) == 0)  return;

  one_sec_time+=1000;

  GetTimeDate(&t);

  WriteNVRAM((int)nr_year   ,(int)(t.year-2000));
  WriteNVRAM((int)nr_month  ,(int)t.month  );
  WriteNVRAM((int)nr_day    ,(int)t.day    );
  WriteNVRAM((int)nr_hour   ,(int)t.hour   );
  WriteNVRAM((int)nr_min    ,(int)t.minute );
  WriteNVRAM((int)nr_sec    ,(int)t.sec    );

  
}
/*-----------------------------------------*/
/*================================================================*/

#include "Queue.c"

#if defined(MMI_NEW)
#include "mmi_new.C"
#endif

#include "Icp.c"
#include "Dos_win.c"
#include "Host.c"
#include "Rtu.c"
#include "Trans.c"

#if defined(ICP_7017C)
#include "7017c.c"
#endif

#include "Eee.c"
#include "Arch.c"
#include "Md5c.c"
#include "UFG.c"
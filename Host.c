int EchoComN=0;
int ComPortHost=4;

int ComPortEcho=1;
int CRC_flag_Echo=1;

int (*ff_putch[5])(int ch)= {ToCom1,ToCom1,ToCom2,ToCom3,ToCom4};

int flag_echo_host=1;
int flag_Deb=0;

unsigned long int adr_strt= 0xffff0L; //-- WORK
//---------------------------------
struct s_icp_dev Host=
{
// device 1
0,                // status
1,                // port
0,                // addr
"Host",           // name[8]
06,               // baudrate_cod
0x00,             // type
0x00,             // format
0x00,             // channels
"V1.1",           // firmware
0,                // protocol
1,                // CRC_flag
150,              // pool_time
0,                // time_stamp_pool;
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&Host_Rqst        // *request
};
//--------------------------------
//---------------------------------
struct COM_rqst Host_Rqst={
0,                     //  состояние запроса
ffgets_com_ICP,        //  gets_com
f_send_to_Host,        //  answ_com
f_empty_ans_echo,      //  answ_flt
"Host",                //  name
0,                     //  n_dev
&Host,                 //  *ICP_dd
0,                     //  текущая функция
ToutAnsHost,           //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
""                     //  com[]
};
/*----------------------------*/
struct COM_rqst Host_Rqst_rtu={
0,                     //  состояние запроса
ffgets_com_rtu,        //  gets_com
f_send_to_Host_rtu,    //  answ_com
f_send_to_Host_rtu,    //  answ_flt
"Host",                //  name
0,                     //  n_dev
&Host,                 //  *ICP_dd
0,                     //  текущая функция
ToutAnsHost_rtu,       //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
""                     //  com[]
};
/*----------------------------*/
struct COM_rqst Host_Rqst_sk={
0,                     //  состояние запроса
ffgets_com_rtu,        //  gets_com
f_send_to_Host_sk,    //  answ_com
f_send_to_Host_sk,    //  answ_flt
"Host",                //  name
0,                     //  n_dev
&Host,                 //  *ICP_dd
0,                     //  текущая функция
ToutAnsHost_rtu,       //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
""                     //  com[]
};
/*----------------------------*/
#define max_wd 2
//---------------
struct s_icp_dev WDOG[max_wd]=
{
// device 1
1,                // status
1,                // port
0,                // addr
"WDOG/1",         // name[8]
06,               // baudrate_cod
00,               // type
0,                // format
0,                // channels
"V1.1",           // firmware
0,                // protocol
1,                // CRC_flag
400,              // pool_time
0,                // time_stamp_pool;
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&WDOG_Rqst[0],   // *request
//--------------------------------
// device 2
//--------------------------------
0,                // status
2,                // port
0,                // addr
"WDOG/2",         // name[8]
06,               // baudrate_cod
00,              // type
00,             // format
0x0,             // channels
"V1.1",           // firmware
0,                // protocol
1,                // CRC_flag
400,              // pool_time
0,                // time_stamp_pool
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&WDOG_Rqst[1],   // *request
};
/*----------------------------*/
/*----------------------------*/
struct COM_rqst WDOG_Rqst[max_wd]={
//---------------------------------------
0,                     //  состояние запроса
ffgets_no_com_ICP,     //  gets_com
f_WD_end,              //  answ_com
f_WD_end,              //  answ_flt
"WDOG/1",              //  name
0,                     //  n_dev
&WDOG[0],              //  *ICP_dd
0,                     //  текущая функция
2,                     //  timeout
0,                     //  time_stamp
6,                     //  com_lgth
1,                     //  CRC_flag
"~**D2\r",             //  com[]
//-------------------------------------
//---------------------------------------
0,                     //  состояние запроса
ffgets_no_com_ICP,     //  gets_com
f_WD_end,              //  answ_com
f_WD_end,              //  answ_flt
"WDOG/2",              //  name
1,                     //  n_dev
&WDOG[1],              //  *ICP_dd
0,                     //  текущая функция
2,                     //  timeout
0,                     //  time_stamp
6,                     //  com_lgth
1,                     //  CRC_flag
"~**D2\r"              //  com[]
//-------------------------------------
};
/*----------------------------*/
void f_WDOG(int ii)
{
  //  ii - номер устройства
  //  Функция передает команду HOST OK ( '~**') .
  //  Период посылки WDOG[ii].pool_time мс.
  //  После передачи ожидается опорожнение буфера и задержка
  //  WDOG_Rqst[ii].timeout мс

int i,fn,sw;
char buf_tmp[20];

//  if(WDOG[ii].status == 0) return;

  if(( WDOG_Rqst[ii].status == Req_OK)||( WDOG_Rqst[ii].status == Req_Flt))
  {
   if( f_timer( WDOG[ii].time_stamp_pool, WDOG[ii].pool_time ))
    {
m1:
     strcpy(buf_tmp,"~**");
     if(f_prepare7000(buf_tmp, WDOG[ii].CRC_flag)) return;
     strcpy( WDOG_Rqst[ii].Cmd,buf_tmp);
     WDOG_Rqst[ii].cmd_lgth=strlen(buf_tmp);
     WDOG_Rqst[ii].time_stamp=TimeStamp;
     WDOG[ii].time_stamp_pool=TimeStamp;
     WDOG[ii].n_transaction++;
     f_queue_put( WDOG[ii].port, &WDOG_Rqst[ii]) ;
    }
  }
  else
   if( f_timer( WDOG[ii].time_stamp_pool, WDOG[ii].pool_time+WDOG[ii].pool_time ))
   {
     if(f_queue_chk(WDOG[ii].port) ) goto m1;
   }
  return;
}
/*----------------------------*/

int host_wd_sw[5]={0,0,0,0,0};
int ffgets_no_com_ICP( int ii)
{
//  Адрес функции будет устанавлен в указателе  ff_gets_com[Port]
//  при посылке команды ~**  в устройства ICP (  7060 и пр.).
//  Функция не ожидает ответа .
//  Если прошло время WDOG_Rqst[ii].timeout мс с момента окончания
// передачи последнего символа функция успешно завершается
//

/* ii - номер COM   1...3 */


   if( !IsTxBufEmpty(ii) )
   {
       host_wd_sw[ii] =0;
       return 0;
   }
   if(host_wd_sw[ii] != 1 )
   {
       time_snd[ii]=TimeStamp;
       host_wd_sw[ii]=1;
   }
   else if (host_wd_sw[ii] == 1 )
   {
        if( f_timer(time_snd[ii],ToutAns[ii]))
        { // истекло время ожидания

           host_wd_sw[ii]=0;
           return 1;
        }
   }
   return 0;
}
/* --------------------------- */
void  f_WD_end (int ii)
{
 struct COM_rqst *request;
 int nn;

  request=COM_que[ii].request[COM_que[ii].empt_ptr];
  nn=request->n_dev; // номер устройства  (0,1..)
  WDOG[nn].n_success++;
  f_queue_free(ii,&WDOG_Rqst[nn]);

}
/* --------------------------- */
int f_123( int n)

/* Проверяет является ли n-ный символ в буфере именем координаты,
   если нет - возвращает -1  , иначе номер координаты по порядку */

{
int i;
           switch(intrpr.wrd[n-1])
           {
            case '1': i=0; goto m_k1;
            case '2': i=1; goto m_k1;
            case '3': i=2; goto m_k1;
            case '4': i=3; goto m_k1;
            case '5': i=4; goto m_k1;
            case '6': i=5; goto m_k1;
            case '7': i=6; goto m_k1;
            case '8': i=7; goto m_k1;
            case '9': i=8; goto m_k1;
            case 'A': i=9; goto m_k1;
            case 'B': i=10; goto m_k1;
            case 'C': i=11; goto m_k1;
            case 'D': i=12; goto m_k1;
            case 'E': i=13; goto m_k1;
            case 'F': i=14; goto m_k1;
           }
           return -1;
   m_k1:   return i;
}
/*----------------------------------------------*/
/*----------------------------*/
int f_host()
{
//  int  hour,min,sec;
     // вводит команду из Host (PC) и интерпретирует ее
         if(ffgets_host(ComPortHost))
         {
          strcpy(intrpr.bfcom,cb_COM[ComPortHost]);
          n_bcom[ComPortHost]=0;
          return  f_intr();
         }
         return 0;
}
/*-------------------------------------------*/
void f_send_to_Host(int ii)
{
/* пересылает принятые из порта ii данные в Host
*/

// char *Result;
// int j,key,nextidx;
// int count;
//  count=n_bcom[ii];  // длина команды

 if(flag_echo_host )
     puts("   ");
 puts(cb_COM[ii]);
 putch('\n\r');
 f_queue_free(ii,&Host_Rqst);
 f_prompt();
}
/*-------------------------------------------*/
void f_prompt()
{
 if(flag_echo_host )
    if(( EchoComN==0)||( EchoComN==4))
     printf("\n\r>");
    else
     printf("\n\rCOM%d>",ComPortEcho);
}

/*-------------------------------------------*/
void f_empty_ans_echo(int ii)
{ // таймаут по ответу

 int count,i;

 if(flag_echo_host )
 {
     printf(" -?- ");
 }
 f_prompt();
 f_queue_free(ii,&Host_Rqst);
}
/*-------------------------------------------*/
/*-------------------------------------------*/
#define max_rd_str 40

int MVD_fl_ord[2]={1,1}; // порядок следования байт для числа float
long int mvd_reg_n=1;
long int mvd_reg_q=1;

void f_send_to_Host_sk(int ii)
{
/* пересылает принятые из SK или MVD данные в Host
*/
 // ii - номер COM порта

 struct COM_rqst *request;
 int i,im,fn,i1;
 char buf_tmp[max_rd_str];
 int count;
 unsigned long int val;
 union  { float f; unsigned long l; } o;

  request=COM_que[ii].request[COM_que[ii].empt_ptr];
  fn=request->function;

  count=n_bcom[ii];  // длина команды

  if(count<3) goto m_err;

  if(f_check_crc_RTU(cb_COM[ii],count)==0)
     goto m_err;
 switch(fn)
 {
  case RD_INT:
     if(cb_COM[ii][1] != 3) goto m_err;
     if(cb_COM[ii][2] != 2) goto m_err;
     val=(  (unsigned long int)cb_COM[ii][4] & 0x00ff) |
         ( ((unsigned long int)cb_COM[ii][3] << 8) & 0x00ff00) ;
     printf("  %ld (0x%lX)",val,val);
     break;
  case RD_LONG:
     if(cb_COM[ii][1] != 3) goto m_err;
     if(cb_COM[ii][2] != 4) goto m_err;
     val=
         ( (unsigned long)(cb_COM[ii][6]) & 0x000000ff) |
         ( ((unsigned long)cb_COM[ii][5] << 8) &0xff00) |
         ( ((unsigned long)cb_COM[ii][4] << 16) &0xff0000)  |
         ( ((unsigned long)cb_COM[ii][3] << 24) &0xff000000);
     printf(" %ld (0x%lX)",val,val);
     break;
  case RD_COIL:
     if(cb_COM[ii][1] != 1) goto m_err;
     im=cb_COM[ii][2];
     i1=3;
     for(i=0;i<im ;i++,i1++)
     {
       printf(" %02X",cb_COM[ii][i1]&0xff);
     }
     break;
  case RD_FLOAT:
      //
     if(cb_COM[ii][1] != 3) goto m_err;
     if(cb_COM[ii][2] != 4) goto m_err;

     o.f=f_get_float( cb_COM[ii], MVD_fl_ord[0]);


/* ============================
     switch(MVD_fl_ord[0])  // ( Reg 0521 )
     {
     case 0:
    //  code == 0 : 0  1  2  3
    //             [3][4][5][6]
     o.l=
         ( (unsigned long)cb_COM[ii][6]  &0xff) |                // 3
         ( ((unsigned long)cb_COM[ii][5] << 8) &0xff00) |        // 2
         ( ((unsigned long)cb_COM[ii][4] << 16) &0xff0000)  |    // 1
         ( ((unsigned long)cb_COM[ii][3] << 24) &0xff000000);    // 0
        break;
     case 1:
    //  code == 1 : 2  3  0  1
    //             [3][4][5][6]
     o.l=
         ( (unsigned long)cb_COM[ii][4]  &  0x000000ff) |         // 3
         ( ((unsigned long)cb_COM[ii][3] << 8) &  0x0000ff00) |   // 2
         ( ((unsigned long)cb_COM[ii][6] << 16) & 0x00ff0000)  |  // 1
         ( ((unsigned long)cb_COM[ii][5] << 24) & 0xff000000);    // 0
         break;
      case 2:
    //  code == 2 : 1  0  3  2
    //             [3][4][5][6]

     o.l=
         ( (unsigned long)cb_COM[ii][5]  &0xff) |              // 3
         ( ((unsigned long)cb_COM[ii][6] << 8) &0xff00) |      // 2
         ( ((unsigned long)cb_COM[ii][3] << 16) &0xff0000)  |  // 1
         ( ((unsigned long)cb_COM[ii][4] << 24) &0xff000000);  // 0
         break;
      case 3:
    //  code == 3 : 3  2  1  0
    //             [3][4][5][6]
     o.l=( (unsigned long)(cb_COM[ii][3]) & 0x000000ff) |      // 3
         ( ((unsigned long)cb_COM[ii][4] << 8) &0xff00) |      // 2
         ( ((unsigned long)cb_COM[ii][5] << 16) &0xff0000)  |  // 1
         ( ((unsigned long)cb_COM[ii][6] << 24) &0xff000000);  // 0
       break;
     }
======================*/
//  printf(">> %02X",cb_COM[ii][3]);
//  printf(" %02X",cb_COM[ii][4]);
//  printf(" %02X",cb_COM[ii][5]);
//  printf(" %02X<<\n\r",cb_COM[ii][6]);

    if(( o.l == 0x7fa00000) || ( o.l == 0x00007fa0))
       printf(" !bad process variable (%08lX)",o.l);
    else
     //  printf(" %9g",o.f);
       printf(" %f",o.f);
     //printf(" %g (%08lX)",o.f,o.l);
    break;

  case RD_STRING:

     if(cb_COM[ii][1] != 3) goto m_err;
     im=cb_COM[ii][2];
     i1=3;
     for(i=0;(i<im)&&(i< (max_rd_str-1)) ;i++,i1++)
     {
      buf_tmp[i]=cb_COM[ii][i1];
     }
     buf_tmp[i]=0;
     printf(" %s",buf_tmp);
     break;

  case WR_INT:
    if(cb_COM[ii][1] != 6) goto m_err;
    break;
  case WR_LONG:
  case WR_FLOAT:
  case WR_STRING:
    if(cb_COM[ii][1] != 16) goto m_err;
    break;

  case WR_COIL:
    if(cb_COM[ii][1] != 5) goto m_err;
    break;

 default: goto m_err;
 }
     f_prompt();
     f_queue_free(ii,&Host_Rqst_sk);
     return;

    // ничего не послано в ответ
m_err:


    if(cb_COM[ii][1] & 0x80 )
    {
      if(cb_COM[ii][2]==1) {printf(" !! Illegal function");goto m2;}
      if(cb_COM[ii][2]==2) {printf(" !! Illegal data address");goto m2;}
      if(cb_COM[ii][2]==3) {printf(" !! Illegal data value");goto m2;}
      if(cb_COM[ii][2]==6) {printf(" !! Busy, rejected message");goto m2;}
    }
/*
    if(count>0) printf(" !! ");
    for(i=0;i<count;i++)
     printf(" %02X",cb_COM[ii][i]&0xff);
*/

      printf(" -?- ");
m2:
     f_prompt();
     f_queue_free(ii,&Host_Rqst_sk);
}
/*-------------------------------------------*/
void f_send_to_Host_rtu(int ii)
{
/* пересылает принятые из порта ii данные в Host
*/

// char *Result;
// int j,key,nextidx;
// int count;
//  count=n_bcom[ii];  // длина команды

 int count,i;

 count=n_bcom[ii];  // длина команды

 if(flag_echo_host )
 {
   if(count)
   {
       puts("   ");
    for(i=0;i<count;i++)
       printf(" %02X",(cb_COM[ii][i])&0xff );
       f_prompt();
//      printf("\n\rCOM%d>",ComPortEcho);
   }
   else
    { // ничего не послано в ответ
       printf(" -?- ");
       f_prompt();
    }
 }
 else
 {
  for(i=0;i<count;i++)
     putch(Host_Rqst.Cmd[i]);
 }

 f_queue_free(ii,&Host_Rqst_rtu);
}
/* ---------------------------------------- */
int ffgets_host( int port)
{
//  Функция принимает команду от Host контроллера (PC)
//  выходная строка  cb_COM[port];
//  длина строки   n_bcom[port];

 char tmp;
m1:
 if(!IsCom(port))
 {
  return 0;
 }
 tmp=ReadCom(port);
   if( tmp == '\b')
    {
     if(n_bcom[port])
      {
       if(flag_echo_host )
        {
         (*ff_putch[port])(tmp);
         (*ff_putch[port])(' ');
         (*ff_putch[port])(tmp);
        }
       n_bcom[port]--;
      }
     goto m1;
    }
   else
    {
//     if ((tmp >= 'a') && (tmp <= 'z')) tmp &= 0xdf;
     cb_COM[port][n_bcom[port]]=tmp;

     if( (++n_bcom[port]) >= n_bufcom )
      {
        cb_COM[port][n_bufcom-1]=0;
        n_bcom[port]=n_bufcom-1;
      }
    }
   switch(tmp)
   { // указать все терминаторы
 //   case 0x0:
 //   case ';':
    case '\n':
    case '\r':
    n_bcom[port]--;
    cb_COM[port][n_bcom[port]]=0;
    return 1;
   }
   if(flag_echo_host)
         (*ff_putch[port])(tmp);
   goto m1;
  }

/* ================================================== */
/* ------------------------------------------- */
struct intr_r intrpr={0,0,0,0,0,0,0,0,0};
/* ========================================================= */
int f_inw()

/*   Из строки intrpr.bfcom   вводится слово в строку intrpr.wrd
   Начало анализа начинается с символа с индексом  intrpr.i3.
     Начальные пробелы и разделители игнорируются , слово вводится до первого
   пробела или разделителя за непробельным символом .
     Указатель  intrpr.i1 устанавливается на первый символ введенного
   слова в intrpr.bfcom.
     Указатель  intrpr.i2 устанавливается на последний символ введенного
   слова в intrpr.bfcom.
     Указатель  intrpr.i3 устанавливается на первый символ не пробел и не
   запятая  в intrpr.bfcom.
     В intrpr.wdl - длина введенного слова .
     Возвращается 0 если нет слова и достигнут конец строки  иначе 1.
*/

{
        intrpr.i1=intrpr.i3;
        intrpr.wdl=0;

//      printf("\n\r f_inw(),intrpr.i3=%d",intrpr.i3);

sw0:    switch(intrpr.bfcom[intrpr.i1])
        {
         case ' ':
         case 0x9:
         case ',':
         case ':':
         case ';':
         case '>':
         case '"':
         case '<':
         case '=':
         case '\n\r':
         case '\r': intrpr.i1++;goto sw0;

/*       case '*':  */
         case 0  :
//     printf("\n\r f_inw(),return(0),intrpr.i1=%d,intrpr.i1=%d,intrpr.i3=%d",intrpr.i1,intrpr.i2,intrpr.i3);
              return 0;
        }

        intrpr.i2=intrpr.i1;

sw1:    switch(intrpr.bfcom[intrpr.i2])
        {
         case ' ':
         case 0x9:
         case ',':
         case ':':
         case ';':
         case '=':
         case '\r':
         case '\n\r':
         case 0  : goto m3;
        }

         intrpr.wrd[intrpr.wdl]=intrpr.bfcom[intrpr.i2];
         if(++intrpr.wdl==wrd_w)  intrpr.wdl-- ;
         intrpr.i2++;
         goto sw1;

m3:      intrpr.i3=intrpr.i2;
         intrpr.i2--;
         intrpr.wrd[intrpr.wdl]=0;

sw2:    switch(intrpr.bfcom[intrpr.i3])
        {
         case ',':
         case ' ':
         case 0x9:
         case '\n\r':
         case '\r': intrpr.i3++;goto sw2;
        }

//      printf("\n\r f_inw(),return(1),intrpr.i3=%d",intrpr.i3);
        return 1;

}
/* ========================================================= */
int f_innum(char *strg)
/* вводит число в intrr[pp].num_var  */
{
    char cp;
    char *strg1;
    char *stopptr;
    float tmp;
    int i1;
    tmp=1.0;

//---------------------
            i1=0;
            if( strg[0] == '-')
             {
               tmp=-1.0;
               i1=1;
             }
            else if( strg[0] == '~')
             {
               tmp=-2.0;
               i1=1;
             }
//---------------------

    if( (strg[i1] == '0') && (strg[i1+1] == 'x')  || (strg[i1+1] == 'X') )
    {
      strg1=&strg[i1+2];
      while(*strg1)
       {
         cp= *strg1++;
         if( isxdigit(cp) );
         else return 0;
       }

      strg1=&strg[i1];
      if(tmp!=-2.0)
        intrpr.num_var=tmp*(float)strtoul(strg1,&stopptr,16);
      else
       {
        intrpr.num_var=(~(strtoul(strg1,&stopptr,16))) & HEX_MAX; ;
       }

      return 1;
    }
    else
    {
      strg1=&strg[i1];
      while(*strg1)
       {
         cp= *strg1++;
         if( isdigit(cp) || (cp=='+')||(cp=='.')||(cp==' ') || (cp=='-'));
         else return 0;
       }
      strg1=&strg[i1];
      if(tmp!=-2.0)
       {
//       intrpr.num_var = tmp *(float)atof(strg1);
         intrpr.num_var = tmp *(float)strtod(strg1,NULL);

       }
      else
       {
       //  tmp= (float)atof(strg1);
         tmp= (float)strtod(strg1,NULL);

          intrpr.num_var=( ( ~((int)tmp)) ) & HEX_MAX;
       }
    }
    return 1;
}
/*---------------------------------------------------------------------*/
int f_inhex(char *strg)
/* вводит hex число в intrr[pp].num_var  */
{
    char cp;
    char *strg1;
    char *stopptr;

      strg1=strg;

      while(*strg1)
       {
         cp= *strg1++;
         if( isxdigit(cp) );
         else return 0;
       }

      strg1=strg;
      intrpr.num_var=(float)strtoul(strg1,&stopptr,16);

    return 1;
}
/*---------------------------------------------------------------------*/
/*----------------------------------------*/
float f_lim(float vvr,float lim_low,float lim_high)
{
  if(vvr > lim_high) vvr=lim_high;
  else if(vvr < lim_low) vvr=lim_low;
  return vvr;
}
/*----------------------------------------*/
/*---------------------------------------------------*/
/* Отображение/ввод значений */
int f_dis_set( struct dis_set *ds_mas,int ib,int nn)
/*
      ib - длина команды
      nn - количество параметров
*/
{
int i,i_rez,i1;
char *strg;
char str_tmp[15];
long int l_tmp;
    i_rez=0;
    if(intrpr.wdl==ib)
       if(f_inw()) strg=intrpr.wrd;
       else
        {
         for(i=0;i<nn;i++)
         switch(ds_mas[i].type)
         {
          case T_FLOAT:
             printf(" %g", *(float*)ds_mas[i].op );
//           printf(" %f", *(float*)ds_mas[i].op );
             break;
          case T_INT:
             printf(" %d", *(int*)ds_mas[i].op );
             break;
          case T_INT_L:
             printf(" %ld", *(long int*)ds_mas[i].op );
             break;
          case T_HEX:
             printf(" %X", *(int*)ds_mas[i].op );
             break;
          case T_HEX_L:
             printf(" %08lX", *(long int*)ds_mas[i].op );
             break;
          case T_HX2:
             printf(" %02X", *(int*)ds_mas[i].op );
             break;
          case T_HX4:
             printf(" %04X", *(int*)ds_mas[i].op );
             break;
          case T_STR:
             printf(" %s", (char*)ds_mas[i].op );
             break;

         }
         return 0;
        }
    else
        strg=&intrpr.wrd[ib];


    switch(ds_mas[0].type)
     {
         case T_FLOAT:
         case T_INT:
          i1=f_innum(strg);
          break;
         case T_INT_L:
          i1=sscanf(strg,"%ld",&l_tmp);
          break;
        case T_HEX_L:
          i1=sscanf(strg,"%lx",&l_tmp);
          break;
        case T_HEX:
        case T_HX2:
        case T_HX4:
          i1=f_inhex(strg);
          break;
        case T_STR:
          i1=1;
          break;
     }
    if(i1)
     {
        switch(ds_mas[0].type)
       {
        case T_FLOAT:
          *(float*)ds_mas[0].op= (float)f_lim(intrpr.num_var,ds_mas[0].low_l,ds_mas[0].high_l);
          break;
        case T_INT:
        case T_HEX:
        case T_HX2:
        case T_HX4:
          *(int*)ds_mas[0].op= (int)f_lim(intrpr.num_var,ds_mas[0].low_l,ds_mas[0].high_l);
          break;
        case T_INT_L:
        case T_HEX_L:
             if((float)l_tmp < ds_mas[0].low_l) l_tmp =ds_mas[0].low_l;
             else if((float)l_tmp > ds_mas[0].high_l) l_tmp=ds_mas[0].high_l;
             *(long int*)ds_mas[0].op=l_tmp;
          break;
        case T_STR:
          strcpy((char*)ds_mas[0].op, strg);
          break;
       }
       i_rez++;
     }
    else goto dis_e;

    if (nn==1) return i_rez;

    for(;i_rez < nn;i_rez++)
     {
       if(!f_inw()) return i_rez;

       switch(ds_mas[i_rez].type)
        {
            case T_FLOAT:
            case T_INT:
             i1=f_innum(intrpr.wrd);
             break;

           case T_INT_L:
            i1=sscanf(intrpr.wrd,"%ld",&l_tmp);
            break;
        case T_HEX_L:
            i1=sscanf(intrpr.wrd,"%lx",&l_tmp);
            break;
           case T_HEX:
           case T_HX2:
           case T_HX4:
             i1=f_inhex(intrpr.wrd);
             break;
           case T_STR:
             i1=1;
             break;
        }
       if(i1)
        {
           switch(ds_mas[i_rez].type)
          {
           case T_FLOAT:
             *(float*)ds_mas[i_rez].op= (float)f_lim(intrpr.num_var,ds_mas[i_rez].low_l,ds_mas[i_rez].high_l);
             break;
           case T_INT:
           case T_HEX:
           case T_HX2:
           case T_HX4:
             *(int*)ds_mas[i_rez].op= (int)f_lim(intrpr.num_var,ds_mas[i_rez].low_l,ds_mas[i_rez].high_l);
             break;
        case T_INT_L:
        case T_HEX_L:
             if(l_tmp < ds_mas[i_rez].low_l) l_tmp =ds_mas[i_rez].low_l;
             else if(l_tmp > ds_mas[i_rez].high_l) l_tmp=ds_mas[i_rez].high_l;
             *(long int*)ds_mas[i_rez].op=l_tmp;
          break;
           case T_STR:
             strcpy((char*)ds_mas[i_rez].op, intrpr.wrd);
             break;
          }
        }
       else goto dis_e;
     }

 // if(f_inw()) goto dis_e;
    return i_rez;

dis_e:
//    (*ff_putch[ComPortHost])('?');
    putch('?');
    return -1;
}
/*------------------------------*/

// сканирование магистрали

int licp_sw=0;
int licp_addr;
int licp_fnd;
int flag_print_licp=0;
int licp_port=1;
//----------------------------

int ToutLicpScan = 30;
int licp_amax=25;  //QQQQ
int licp_crc_flag=1;
int licp_t_conf=0;

struct icp_mod ICP_MOD[5][icp_mod_max];
int  ICP_mod_fnd[5]={0,0,0,0,0};

//----------------------------
void f_look_ICP()
{
int i;
char buf_tmp[20];
switch(licp_sw)
 {  /* сканирование ICP slave контроллеров на магистрали */
   case 0: break;
   case 1:
          licp_addr=0;
          licp_fnd=0;
          licp_sw=2;
          LICP_Rqst.answ_com=f_result_Licp;
          LICP_Rqst.answ_flt=f_empty_ans_Licp;
          LICP_Rqst.timeout=ToutLicpScan;

   case 2:
      if(licp_addr>=licp_amax)
      { // сканирование закончено
         ICP_mod_fnd[licp_port]=licp_fnd;
         if(licp_fnd)
         {
            LICP_Rqst.answ_com=f_ans_Licp_cfg;
            LICP_Rqst.answ_flt=f_ans_Licp_Flt;
            licp_t_conf=0;

            LICP_Rqst.timeout=ToutLicpScan*5;
            licp_sw=3;
            if(flag_print_licp) puts("\n\r");
            break;
         }
         else
         {
           if(flag_print_licp)
           {
             printf("\n\r COM%d : устройства ICP не найдены\n\r",licp_port);
           }
           if(ff_serv==f_look_ICP) ff_serv=NULL;
           licp_sw=0;
           break;
         }
      }
      if( f_queue_chk(licp_port) && (LICP_Rqst.status == Req_OK) )
      {
       // послать команду "read name" по текущему адресу
           sprintf(buf_tmp,"$%02XM",licp_addr);
           f_prepare7000(buf_tmp,LICP_Rqst.CRC_flag);
           strcpy(LICP_Rqst.Cmd,buf_tmp);
           LICP_Rqst.cmd_lgth=strlen(LICP_Rqst.Cmd);
           LICP_Rqst.time_stamp=TimeStamp;
           f_queue_put(licp_port, &LICP_Rqst);
      }
      break;

   case 3: // опрос конфигурации найденных устройств
      if(licp_t_conf >= licp_fnd)
      {
           licp_sw=4;
           break;
      }
      _fmemset(&ICP_MOD[licp_port][licp_t_conf].TT,0xaa,sizeof(int)*3);
      if( f_queue_chk(licp_port) && (LICP_Rqst.status == Req_OK) )
      {
       // послать команду опроса конфигурации найденным устройствам
           sprintf(buf_tmp,"$%02X2",ICP_MOD[licp_port][licp_t_conf].addr0);
           f_prepare7000(buf_tmp,LICP_Rqst.CRC_flag);
           strcpy(LICP_Rqst.Cmd,buf_tmp);
           LICP_Rqst.cmd_lgth=strlen(LICP_Rqst.Cmd);
           LICP_Rqst.time_stamp=TimeStamp;
           f_queue_put(licp_port, &LICP_Rqst);
      }
      break;

   case 4:

       if(flag_print_licp && licp_fnd)
       {
          printf("\n\r COM%d,найдены устройства ICP:",licp_port);
          f_mod_list();
       }
       if(ff_serv==f_look_ICP) ff_serv=NULL;
       licp_sw=0;
       break;
 }
 return;
}
//-------------------------------------------------
// структура запроса при сканировании магистрали
//-------------------------------------------------
struct COM_rqst LICP_Rqst={
0,                     //  состояние запроса
ffgets_com_ICP,        //  gets_com
f_result_Licp,         //  answ_com
f_empty_ans_Licp,      //  answ_flt
"SCAN bus",            //  name
0,                     //  n_dev
&Host,                 // *ICP_dd
0,                     //  текущая функция
50,                    //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
""                     //  com[]
};
/*----------------------------*/
/*----------------------------*/
void f_result_Licp(int ii)
{
/* заносит принятые данные в массив структур
    ICP_MOD[]
*/
 int i;
 long int count;
 if(licp_crc_flag==1)
    count=n_bcom[ii]-5;  // длина имени
 else
    count=n_bcom[ii]-3;  // длина имени

//в ответ на $00M принимается  !00MMIFCT41
 if(licp_crc_flag==0) goto m1;
 if(f_check_crc_ICP(cb_COM[ii]))
 {
m1:
  _fmemcpy( ICP_MOD[ii][licp_fnd].name,&cb_COM[ii][3],count);
  ICP_MOD[ii][licp_fnd].name[count]=0;

  cb_COM[ii][3]=0;
  sscanf(&cb_COM[ii][1],"%2x",&i);
  ICP_MOD[ii][licp_fnd].addr=i;
  ICP_MOD[ii][licp_fnd].addr0=licp_addr;
  ICP_MOD[ii][licp_fnd].port=ii;
  licp_fnd++;
 }
 if(licp_fnd == icp_mod_max)
  {
    licp_addr=licp_amax;
  }
  else
    licp_addr++;
 f_queue_free(ii,&LICP_Rqst);
 if(flag_print_licp) putch('x');
}
/*-------------------------------------------*/
void f_empty_ans_Licp(int ii)
{ // таймаут по ответу
 licp_addr++;
 f_queue_free(ii,&LICP_Rqst);
 if(flag_print_licp) putch('.');
}
/*-------------------------------------------*/
/*-------------------------------------------*/
void f_ans_Licp_cfg(int ii)
{
/* заносит принятые данные о конфигурации
   в ICP_MOD[]
*/
 char *result;
 int *dat;
 int i;

// в ответ на $002 приходит
//  !00 01 06 40  AC
 result=cb_COM[ii];
 if(licp_crc_flag==0) goto m1;
 if(f_check_crc_ICP(result))
 {
m1:
  sscanf(&result[1],"%2X",&i);
  if(((i != ICP_MOD[ii][licp_t_conf].addr)&& (ICP_MOD[ii][licp_t_conf].addr !=0) )||(*result != '!')) goto m_flt;

  dat=&ICP_MOD[ii][licp_t_conf].TT;
  sscanf(&result[3],"%2x%2x%2x",&dat[0],&dat[1],&dat[2]);
  ICP_MOD[ii][licp_t_conf].port=ii;
 }
m_flt:
 licp_t_conf++;
 f_queue_free(ii,&LICP_Rqst);
// if(flag_print_licp) putch('+');
}
/*-------------------------------------------*/
void f_ans_Licp_Flt(int ii)
{ // таймаут по ответу
 licp_t_conf++;
 f_queue_free(ii,&LICP_Rqst);
// if(flag_print_licp) putch('-');
}
//-------------------------------
/*-------------------------------------------*/
//-------------------------------
void  f_print_que()
{
int i;
    struct COM_rqst *request;
    struct s_icp_dev *ICPl_DEV;
    struct icp_mod *ICPl_MOD;

      printf("\n\r Ports queue:");
      for(i=1;i<4;i++)
      {
        printf("\n\rCOM%d: ",i);
        printf(" busy_ptr=%d,empty_ptr=%d,last_out=%d",COM_que[i].busy_ptr,COM_que[i].empt_ptr,last_out[i]);
        request=COM_que[i].request[COM_que[i].empt_ptr];
        if(request != NULL)
          printf("\n\r   Last srv: %s,status %d",request->name,request->status);
        request=COM_que[i].request[COM_que[i].busy_ptr];
           if(request != NULL)
          printf("\n\r   Last put: %s,status %d",request->name,request->status);
      }
      printf("\n\r ICP device(s):");

 printf(
"\n\rName     Status Port Addr CRC Pool   Send  Success Tout_err CRC_err T_snd T_ans");

      for(i=0;i<icp_lst_max;i++)
      {
        ICPl_DEV=ICP_dd[i];
 printf(
  "\n\r%10s    %1d   %1d   %3d  %d  %d",ICPl_DEV->name,ICPl_DEV->status,ICPl_DEV->port,ICPl_DEV->addr,ICPl_DEV->CRC_flag,ICPl_DEV->pool_time);

 printf("%8ld %8ld %6ld %6ld %6ld %6ld",ICPl_DEV->n_transaction,ICPl_DEV->n_success,ICPl_DEV->n_timeout_error,ICPl_DEV->n_CRC_error,
   ICPl_DEV->time_stamp_snd,ICPl_DEV->time_stamp_rcv);
      }
      printf("\n\r time %.3f sec",(float)(TimeStamp-TimeClear)/1000.);
/*
      GetTime(&hour,&min,&sec);
      printf("  %2d:%2d:%2d" ,hour,min,sec);
*/
}
//-------------------------------
void f_drv_list()
{ // показать список драйверов устройств
int i;
  struct s_icp_dev *ICPl_DEV;
  struct COM_rqst *request;

  printf("\n\rNamb   name       status  port addr CRC_flag pool,ms timeout,ms");
    for(i=0;i<icp_lst_max;i++)
    {
      ICPl_DEV=ICP_dd[i];
      request=ICPl_DEV->request;
      printf("\n\r%2d  %10s      %d      %d   %3d     %d",i+1,ICPl_DEV->name,ICPl_DEV->status,ICPl_DEV->port,ICPl_DEV->addr,ICPl_DEV->CRC_flag);
      printf("      %4ld    %4ld", ICPl_DEV->pool_time,request->timeout);
    }
//    printf("\n\r");
}
//-------------------------------
struct icp_mod *f_get_mod_str(int num)
{ // функция возвращает указатель на структуру модудя
  // с номером num . num = 0...
  //
  int i,i1,nn;
  struct icp_mod *ICPl_MOD;

  nn=num+1;
  i1=ICP_mod_fnd[1]+ICP_mod_fnd[2]+ICP_mod_fnd[3];
  if((i1==0)|| (nn>i1) )return NULL;

  for(i=1;i<4;i++)
  {
   if(nn<= ICP_mod_fnd[i])
    {
      i1=nn-1;
      break;
    }
   else nn-=ICP_mod_fnd[i];
  }
  ICPl_MOD=&ICP_MOD[i][i1];
  return ICPl_MOD;
}
//-------------------------------
void f_mod_list()
{ // показать список обнаруженных модулей
int i,i1,i2;
 // int ICP_mod_fnd[5];
  struct icp_mod *ICPl_MOD;

  i1=ICP_mod_fnd[1]+ICP_mod_fnd[2]+ICP_mod_fnd[3];
  if(i1==0) printf("list empty, use SCAN (SCANP)\n\r");
  else
  {
  printf("\n\rnamb    Name      port adress boud  TT  FF");
  i2=1;
    for(i=0;i<i1;i++)
    {
      ICPl_MOD=f_get_mod_str(i);
      if(ICPl_MOD==0) break;
     if(ICPl_MOD->addr0 != ICPl_MOD->addr)
      printf("\n\r%3d %10s      %d \(%02X\)%02X  %6ld",i2,ICPl_MOD->name,ICPl_MOD->port,ICPl_MOD->addr0,ICPl_MOD->addr,baudrate_val[ICPl_MOD->CC]);
     else
      printf("\n\r%3d %10s      %d    %02X   %6ld",i2,ICPl_MOD->name,ICPl_MOD->port,ICPl_MOD->addr,baudrate_val[ICPl_MOD->CC]);

      printf("   %02X  %02X",ICPl_MOD->TT,ICPl_MOD->FF);
      i2++;
    }
      printf("\n\r");
  }
}
//-------------------------------
//-------------------------------
void  f_icp_hlp()
{
  puts("\n\r ICP modules commands:\n\r");
  puts(" $AAM - read module name\n\r");
  puts(" $AA2 - read module configuration\n\r");
  puts(" ~AAO(Name) - change module name\n\r");
  puts(" %AANNTTCCFF - change module configuration\n\r");
  puts(" $AA5VV - enable analog channels VV (for I7017)\n\r");
  puts(" $AA6 - read channel enable status (for I7017)\n\r");
}


/*----------------------------------------------*/
//----------------------
union
{
char ch[4];
long int lng;
} *lng_chr;

//struct arch_str ar_str0_;

int status,port,addr,CRC_flag,pool_time,timeout;

struct dis_set  ds_dev[]=
{
/*----------------*/
  &status,
  0,
  1,
  T_INT,
/*----------------*/
  &port,
  1,
  3,
  T_INT,
/*----------------*/
  &addr,
  0,
  255,
  T_INT,
/*----------------*/
  &CRC_flag,
  0.,
  1,
  T_INT,
/*----------------*/
  &pool_time,
  0.,
  BIG_I,
  T_INT,
/*----------------*/
  &timeout,
  0.,
  BIG_I,
  T_INT,
/*----------------*/
/*----------------*/
};
long int baud=9600;
int TT_par,FF_par,addr0;
//----------------------
struct dis_set  ds_mod[]=
{
/*----------------*/
  &addr,
  0,
  255,
  T_INT,
/*----------------*/
  &baud,
  1200.,
  115200,
  T_INT_L,
/*----------------*/
  &TT_par,
  0.,
  BIG_I,
  T_HX2,
/*----------------*/
  &FF_par,
  0.,
  BIG_I,
  T_HX2,
/*----------------*/
};
//----------------------
//----------------------
char buf_str[20];
struct dis_set  ds_modn=
{
/*----------------*/
  buf_str,
  1,
  BIG_P,
  T_STR,
/*----------------*/
};
//----------------------
struct dis_set  ds_wins=
{
/*----------------*/
  &FlagWinSum,
  0,
  1,
  T_INT,
//-----------
};
//----------------------
//----------------------
struct dis_set  ds_scan[4]=
{
/*----------------*/
  &ComPortEcho,
  1,
  3,
  T_INT,
/*----------------*/
  &ToutLicpScan,
  0.,
  BIG_I,
  T_INT,
/*----------------*/
  &licp_amax,
  0.,
  255,
  T_INT,
/*--------------*/
  &LICP_Rqst.CRC_flag,
  0,
  1,
  T_INT,
/*----------------*/
};
//----------------------
struct dis_set  ds_mka[]=
{
/*----------------*/
  &analog_num[0],
  0,
  10,
  T_INT,
/*----------------*/
  &analog_num[1],
  0,
  10,
  T_INT,
/*----------------*/
};
//----------------------
float ftmp_na[2];
struct dis_set  ds_na[]=
{
/*----------------*/
  &ftmp_na[0],
  -BIG_P,
  BIG_P,
  T_FLOAT,
/*----------------*/
  &ftmp_na[1],
  -BIG_P,
  BIG_P,
  T_FLOAT,
/*----------------*/
};
//----------------------
struct dis_set  ds_no[]=
{
/*----------------*/
  &analog_offset[0],
  -BIG_P,
  BIG_P,
  T_FLOAT,
/*----------------*/
  &analog_offset[1],
  -BIG_P,
  BIG_P,
  T_FLOAT,
/*----------------*/
};
//----------------------
struct dis_set  ds_pass[]=
{
/*----------------*/
  &Password,
  0,
  BIG_P,
  T_INT_L,
/*----------------*/
  &Password_m,
  0,
  BIG_P,
  T_INT_L,
/*----------------*/
};
//----------------------
int cm_lgth,cm_parity,cm_stop;
long int cm_baud=9600;
struct dis_set  ds_cm[]=
{
/*----------------*/
  &cm_baud,
  1200,
  115200L,
  T_INT_L,
/*----------------*/
  &cm_lgth,
  7,
  8,
  T_INT,
/*----------------*/
  &cm_parity,
  0,
  4,
  T_INT,
/*----------------*/
  &cm_stop,
  1,
  2,
  T_INT,
/*----------------*/
};
//----------------------
struct dis_set  ds_cp[4]=
{
/*----------------*/
  &ComProt[1],
  0,
  3,
  T_INT,
/*----------------*/
  &ComProt[2],
  0,
  3,
  T_INT,
/*----------------*/
  &ComProt[3],
  0,
  3,
  T_INT,
/*----------------*/
  &MVD_fl_ord[0],
  0,
  3,
  T_INT,
/*----------------*/
};
//----------------------
struct dis_set  ds_rmvd[2]=
{
/*----------------*/
  &mvd_reg_n,
  1.,
  65536,
  T_INT_L,
/*----------------*/
  &mvd_reg_q,
  0.,
  65535,
  T_INT_L,
/*----------------*/
};
//----------------------
unsigned int fle;
struct dis_set  ds_fle=
{
/*----------------*/
  &fle,
  /*0xC000,*/
  0xB000,
  0xE000,
  T_HX4,
/*----------------*/
};
//---------------------------------
unsigned int seg=0xB000,offset=0;
unsigned char value_flash = 0x12;
struct dis_set  ds_vso[]=
{
//----------------
  &offset,
  0,
  65535,
  T_INT,
//----------------
  &seg,
  0xB000,
  0xE000,
  T_HX4,
//----------------
};
//-----------------------------------

int f_intr()
{
  int i,i1,i2,i3,j,fn;
  long int i_l,i_l1,i_l2;
  char *ch_ptr;
  struct COM_rqst *request;
  struct s_icp_dev *ICPl_DEV;
  struct icp_mod *ICPl_MOD;
  char buf_tmp[50];
  char ch_tmp;
  int  hour,min,sec;
  //struct arch_str *a_str;
  float ftmp;
  long int *id_ptr;

  union  { float f; unsigned long l; } o,o1;

        intrpr.i3=0;
        if(f_inw()==0)
        {
          intrpr.n_bcom=0;
          goto fin;
        }

//---------------------------------
        if ((intrpr.wrd[0]=='Q') || (intrpr.wrd[0]=='q'))
        {
            printf ("\n\r QQQ \n\r");
            return 1;
        }
//---------------------------------
      if (!strcmp(intrpr.wrd,"RES" ))
        {   //'RES'


  StopUserTimerFun();
  RestoreCom(1);
  RestoreCom(2);
  RestoreCom(3);
  while(!IsCom4OutBufEmpty()) ;  /* Wait all message sent out of COM4 */
  RestoreCom(4);
  //f_free_SlvBuf();

          _asm cli
          _asm hlt
//        (*(void (*)())adr_strt)();
          goto fin;
        }
//---------------------------------
      if (!strcmp(intrpr.wrd,"MD5" ))
        {   //'MD5'
          f_md5(0);
          goto fin;
        }
//---------------------------------
       if (!strcmp(intrpr.wrd,"WE" ))
       {   //'WE'
        if(f_wrt_eee()<0)
        {
          goto fin_e;
        }
        else
        {
          if(flag_echo_host )
          printf(" %d",ee_members);
        }
        goto fin;
       }
//---------------------------------
      if (!strcmp(intrpr.wrd,"MKA" ))
        {   //'MKA'
          f_dis_set(ds_mka,3,2);
          goto fin;
        }
//---------------------------------
      if (!strcmp(intrpr.wrd,"NA" ))
        {   //'NA'
          ftmp_na[0]=analog_scale[0]*NA_scale;
          ftmp_na[1]=analog_scale[1]*NA_scale;
          if(f_dis_set(ds_na,2,2)>0)
          {
           analog_scale[0]=ftmp_na[0]/NA_scale;
           analog_scale[1]=ftmp_na[1]/NA_scale;
          }
          goto fin;
        }
//---------------------------------
      if (!strcmp(intrpr.wrd,"NO" ))
        {   //'NO'
          f_dis_set(ds_no,2,2);
          goto fin;
        }
//---------------------------------
       if (!strcmp(intrpr.wrd,"CP" ))
        {   //'CP' Com Protocol
          f_dis_set(ds_cp,2,4);
          goto fin;
        }
//---------------------------------
      if (!strcmp(intrpr.wrd,"PASS" ))
        {   //'PASS'Password
          f_dis_set(ds_pass,4,2);
          goto fin;
        }
//------------------------------------------
        if  ((intrpr.wrd[0]=='C') &&(intrpr.wrd[1]=='M') )
        {   //"CM" 'CM' - Com Mode
         if( ((i=f_123(3)) >= 0 ) &&  (i< 5) )
         {
          i++;
          cm_baud  = ComBaud[i]   ;
          cm_lgth  = Com_lgth[i]  ;
          cm_parity= Com_parity[i];
          cm_stop  = Com_stop[i]  ;

          if((i1=f_dis_set(ds_cm,3,4)) >0 )
          {
//           printf("\n\r i= %d, %ld %d %d %d",i,cm_baud,cm_lgth,cm_parity,cm_stop   );
            if(f_get_nb(cm_baud) < 0  ) goto fin_e;

            ComBaud[i]    = cm_baud   ;
            Com_lgth[i]   = cm_lgth   ;
            Com_parity[i] = cm_parity ;
            Com_stop[i]   = cm_stop   ;

            RestoreCom(i);
            if( InstallCom(i,ComBaud[i],Com_lgth[i],Com_parity[i],Com_stop[i]) < 0 ) goto fin_e;
          }

          //ShowCom(ComPortHost,Com_lgth[ComPortHost],ComBaud[ComPortHost]);
          goto fin;
         }
        }
//---------------------------------
        if  ((intrpr.wrd[0]=='D') &&(intrpr.wrd[1]=='R')&&(intrpr.wrd[2]=='V'))
        {   //'DRV'
         if( ((i=f_123(4)) >= 0 ) &&  (i< icp_lst_max ) )
         {
           ICPl_DEV = ICP_dd[i];
           request=ICPl_DEV->request;

           status    = ICPl_DEV->status    ;
           port      = ICPl_DEV->port      ;
           addr      = ICPl_DEV->addr      ;
           CRC_flag  = ICPl_DEV->CRC_flag  ;
           pool_time = ICPl_DEV->pool_time ;
           timeout    = request->timeout;
          if(f_dis_set(ds_dev,4,6) >= 0)
          {
           ICPl_DEV->status    = status    ;
           ICPl_DEV->port      = port      ;
           ICPl_DEV->addr      = addr      ;
           ICPl_DEV->CRC_flag  = CRC_flag  ;
           ICPl_DEV->pool_time = pool_time ;
           request->timeout= timeout;
          }
          goto fin;
         }
        }
//---------------------------------
        if  ((intrpr.wrd[0]=='M') &&(intrpr.wrd[1]=='O') && (intrpr.wrd[2]=='D'))
        {   //'MOD'
         i1=ICP_mod_fnd[1]+ICP_mod_fnd[2]+ICP_mod_fnd[3];
         if( ((i=f_123(4)) >= 0 ) &&  (i< i1 ) )
         {
           ICPl_MOD=f_get_mod_str(i);

           addr0  = ICPl_MOD->addr0;
           addr  = ICPl_MOD->addr;
           baud   = baudrate_val[ICPl_MOD->CC];
           TT_par = ICPl_MOD->TT ;
           FF_par = ICPl_MOD->FF ;

          if((i2=f_dis_set(ds_mod,4,4)) > 0)
          {
            i=f_get_nb(baud);
            if(i<0) goto fin_e;

            ICPl_MOD->addr =  addr  ;
            ICPl_MOD->TT   =  TT_par ;
            ICPl_MOD->CC   = i;
            ICPl_MOD->FF   =  FF_par ;
            if( f_queue_chk(ICPl_MOD->port ) )
            {
             // послать команду "set cfg" по старому адресу устройства
               sprintf(buf_tmp,"%c%02X%02X%02X%02X%02X",'%',addr0,ICPl_MOD->addr,ICPl_MOD->TT,ICPl_MOD->CC,ICPl_MOD->FF);
               f_prepare7000(buf_tmp,LICP_Rqst.CRC_flag);
               strcpy(Host_Rqst.Cmd,buf_tmp);
               Host_Rqst.cmd_lgth=strlen(Host_Rqst.Cmd);
               Host_Rqst.time_stamp=TimeStamp;
               f_queue_put(ICPl_MOD->port, &Host_Rqst);
               return 0;
            }
          }
          goto fin;
         }
        }
//---------------------------------
        if  ((intrpr.wrd[0]=='M') &&(intrpr.wrd[1]=='O')&&(intrpr.wrd[2]=='D')&&(intrpr.wrd[3]=='N'))
        {   //'MODN'
         i1=ICP_mod_fnd[1]+ICP_mod_fnd[2]+ICP_mod_fnd[3];
         if( ((i=f_123(5)) >= 0 ) &&  (i< i1 ) )
         {
           ICPl_MOD=f_get_mod_str(i);

           strcpy(buf_str,ICPl_MOD->name);

          if((i2=f_dis_set(&ds_modn,5,1)) > 0)
          {

           strcpy(ICPl_MOD->name,buf_str);
           if( f_queue_chk(ICPl_MOD->port ) )
            {
             // послать команду set name
               sprintf(buf_tmp,"~%02XO%s",ICPl_MOD->addr,ICPl_MOD->name);
               f_prepare7000(buf_tmp,LICP_Rqst.CRC_flag);
               strcpy(Host_Rqst.Cmd,buf_tmp);
               Host_Rqst.cmd_lgth=strlen(Host_Rqst.Cmd);
               Host_Rqst.time_stamp=TimeStamp;
               f_queue_put(ICPl_MOD->port, &Host_Rqst);
               return 0;
            }
          }
          else if(i2==0);
          else goto fin;

          goto fin;
         }
        }
//---------------------------------
     if  ((intrpr.wrd[0]=='S') &&(intrpr.wrd[1]=='C')&&(intrpr.wrd[2]=='A')&&(intrpr.wrd[3]=='N') &&(intrpr.wrd[4]=='P'))
   //    if (!strcmp(intrpr.wrd,"SCANP" ))
        {   //'SCANP'
          f_dis_set(ds_scan,5,4);
          goto fin;
        }
//---------------------------------
   if (!strcmp(intrpr.wrd,"SCAN" ))
   {    //'SCAN'

    if(ComProt[ComPortEcho]==0)
    {
      licp_sw=1;
      licp_port=ComPortEcho;
      ff_serv=f_look_ICP;
      flag_print_licp=1;
      LICP_Rqst.timeout=ToutLicpScan;
      printf("\n\r Scanning ICP COM%d: tout %d ms, max addr %d,CRC_CHK=%d",ComPortEcho,ToutLicpScan,licp_amax,LICP_Rqst.CRC_flag);
      licp_crc_flag=LICP_Rqst.CRC_flag;
    }
    else
    {
      /*licp_sw=1;
      licp_port=ComPortEcho;
      ff_serv=f_look_RTU;
      flag_print_licp=1;
      LICP_Rqst_RTU.timeout=ToutLicpScan;
      printf("\n\r Scanning RTU COM%d: tout %d ms, max addr %d",ComPortEcho,ToutLicpScan,licp_amax);
    */
    }
    goto fin;
   }
//---------------------------------
      if (!strcmp(intrpr.wrd,"SQ" ))
          {   // 'SQ '
           f_print_que();
           goto fin;
          }
//---------------------------------
      if (!strcmp(intrpr.wrd,"HELP" ))
          {   // 'HELP '
           f_icp_hlp() ;
           goto fin;
          }
//---------------------------------
//---------------------------------
   if (!strcmp(intrpr.wrd,"DRV" ))
          { // 'DRV' - drivers list
            f_drv_list();
            goto fin;
          }
//---------------------------------
   if (!strcmp(intrpr.wrd,"MOD" ))
          { // 'MOD' - modules list
            f_mod_list();
            goto fin;
          }
//---------------------------------
//---------------------------------
/*      if (!strcmp(intrpr.wrd,"CE" ))
        {   //'CE'
         f_cl_error();
         goto fin;
        }*/
//---------------------------------
//---------------------------------
      if (!strcmp(intrpr.wrd,"VER" ))
        {   //'VER'
         printf(" %s",sw_ver);
         goto fin;
        }
//---------------------------------
//---------------------------------
//---------   UFG   ---------------
//---------------------------------
      if (!strcmp(intrpr.wrd,"RUI" ))
        {   //'RUI' Read UFG Integer
          mvd_reg_q=1;  // количество читаемых регистров
          j=1;          // количество вводимых параметров
          i2=RD_INT;
          fn=3;
          goto m_rml;
        }
//---------------------------------
      if (!strcmp(intrpr.wrd,"RUL" ))
        {   //'RUL' Read UFG Long
          i2=RD_LONG;
          j=1;
          fn=3;
          mvd_reg_q=2;
 m_rml:
          if(f_dis_set(ds_rmvd,3,j)>= 0)
          { //sk_reg_n = адрес+1
             buf_tmp[0]=UFG[0].addr;
             buf_tmp[1]=fn;
             buf_tmp[2]=((mvd_reg_n-1)>>8) & 0xff;
             buf_tmp[3]=(mvd_reg_n-1)  & 0xff;
             buf_tmp[4]=(mvd_reg_q>>8) & 0xff;
             buf_tmp[5]=mvd_reg_q & 0xff;
             Host_Rqst_sk.function=i2;

             i1=6;
             f_prepareRTU(buf_tmp,&i1);
             _fmemcpy(Host_Rqst_sk.Cmd,buf_tmp,(long int)i1);
             Host_Rqst_sk.cmd_lgth=i1;
             Host_Rqst_sk.time_stamp=TimeStamp;
             if( f_queue_put(UFG[0].port, &Host_Rqst_sk)) goto fin_e;
          }
          return 0;
        }
//---------------------------------
      if (!strcmp(intrpr.wrd,"RUF" ))
        {   //'RUL' Read UFG Float
          i2=RD_FLOAT;
          mvd_reg_q=2;
          fn=3;
          j=1;
          goto m_rml;
        }
//---------------------------------
      if (!strcmp(intrpr.wrd,"FLE" ))
        {   //'FLE'
         if( f_dis_set(&ds_fle,3,1))
          {
           seg=fle;
           if( FlashErase(seg) != 0) goto fin_e;
          }
          goto fin;
        }
//------------------------------------------
      if (!strcmp(intrpr.wrd,"VSO" )) //VALUE SEG OFFSET
        {   //'VSO'
          f_dis_set(ds_vso,3,2);
          goto fin;
        }
//---------------------------------
      if (!strcmp(intrpr.wrd,"VALU" )) //VALUE SEG OFFSET
        {   //'VALU'
          printf (" %x",value_flash);
          goto fin;
        }
//---------------------------------
fin_e:
         if(( EchoComN==0)||( EchoComN==4))
         {
          if(flag_echo_host )
           printf("?\n\r>");
           return 0;
         }
         else
         { // посылка неопознанной команды в COM (ComPortEcho)
      //   ComPortEcho=EchoComN;

          if(ComProt[ComPortEcho]== ICP)
          {
           if( f_queue_chk(ComPortEcho) && (Host_Rqst.status == Req_OK) )
           {
              { // стать в очередь доступа к COM

                f_prepare7000(intrpr.bfcom,LICP_Rqst.CRC_flag);
                strcpy(Host_Rqst.Cmd,intrpr.bfcom);
                Host_Rqst.timeout= ToutLicpScan;
                Host_Rqst.cmd_lgth=strlen(Host_Rqst.Cmd);
                Host_Rqst.time_stamp=TimeStamp;
                f_queue_put(ComPortEcho, &Host_Rqst);
              }
           }
           else printf(" -- no place in queue COM%d-- \n\r",ComPortEcho);
          }
          else if(ComProt[ComPortEcho]== RTU)
          {
           if( f_queue_chk(ComPortEcho) && (Host_Rqst_rtu.status == Req_OK) )
           {
              { // стать в очередь доступа к COM

                if( (i1=f_chg_toRTU(intrpr.bfcom)) > 0 )
                {
                  f_prepareRTU(intrpr.bfcom,&i1);
                  _fmemcpy(Host_Rqst_rtu.Cmd,intrpr.bfcom,(long int)i1);
                  Host_Rqst_rtu.cmd_lgth=i1;
                  Host_Rqst_rtu.time_stamp=TimeStamp;
                  f_queue_put(ComPortEcho, &Host_Rqst_rtu);
                }
                else goto fin_ee;
              }
           }
           else
           {
             printf(" -- no place in queue COM%d-- \n\r",ComPortEcho);
             goto fin_ee;
           }
          }
          else if(ComProt[ComPortEcho]== 2)
          {  // передача в канал без добавления контрольной суммы
           if( f_queue_chk(ComPortEcho) && (Host_Rqst_rtu.status == Req_OK) )
           {
              { // стать в очередь доступа к COM

                if( (i1=f_chg_toRTU(intrpr.bfcom)) > 0 )
                {
           //     f_prepareRTU(intrpr.bfcom,&i1);
                  _fmemcpy(Host_Rqst_rtu.Cmd,intrpr.bfcom,(long int)i1);
                  Host_Rqst_rtu.cmd_lgth=i1;
                  Host_Rqst_rtu.time_stamp=TimeStamp;
                  f_queue_put(ComPortEcho, &Host_Rqst_rtu);
                }
                else goto fin_e;
              }
           }
           else
           {
             printf(" -- no place in queue COM%d-- \n\r",ComPortEcho);
             goto fin_ee;
           }

          }
          else if(ComProt[ComPortEcho]== 3)
          {  // передача в канал символьной информации без добавления контрольной суммы
             // в конце строки добавляется 0xd
           if( f_queue_chk(ComPortEcho) && (Host_Rqst.status == Req_OK) )
           {
              { // стать в очередь доступа к COM

                strcpy(Host_Rqst.Cmd,intrpr.bfcom);
                Host_Rqst.timeout= ToutLicpScan;
                i1=strlen(Host_Rqst.Cmd);
                Host_Rqst.Cmd[i1]=0xd;
                Host_Rqst.Cmd[i1+1]=0;
                Host_Rqst.cmd_lgth=i1+1;
                Host_Rqst.time_stamp=TimeStamp;
                f_queue_put(ComPortEcho, &Host_Rqst);
              }
           }
           else
           {
             printf(" -- no place in queue COM%d-- \n\r",ComPortEcho);
             goto fin_ee;
           }
          }

         }
         return 0;
fin_ee:
 if(flag_echo_host )
   putch('?');
fin:
 if(flag_echo_host )
         if(( EchoComN==0)||( EchoComN==4))
          printf("\n\r>");

         else
          printf("\n\rCOM%d>",ComPortEcho);
          return 0;

     }
/* ---------------------------------------- */
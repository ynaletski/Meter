
#define Press_mb
///qqq///////////////////////////////////
#define max_ufg 2

struct s_UFG_dev s_UFG =
{
-1, //dens
-1, //temp
-1, //vol
};

//---------------
struct s_icp_dev UFG[max_ufg]=
{
// device 1
0,                // status
2,                // port
1,               // addr
"UFG",        // name[8]
06,               // baudrate_cod
0x00,              // type
0x00,             // format
0x00,             // channels
"B2.2",           // firmware
1,                // protocol
1,                // CRC_flag
400,              // pool_time
0,                // time_stamp_pool;
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&UFG_Rqst[0],   // *request
//--------------------------------
// device 2
//--------------------------------
0,                // status
1,                // port
3,               // addr
"UFG/2",        // name[8]
06,               // baudrate_cod
0x00,              // type
0x00,             // format
0x00,             // channels
"B2.2",           // firmware
1,                // protocol
1,                // CRC_flag
200,              // pool_time
0,                // time_stamp_pool
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&UFG_Rqst[1],   // *request
};
/*----------------------------*/
struct COM_rqst UFG_Rqst[max_ufg]={
//----------------------
// device 1
//--------------------------
0,                     //  состояние запроса
ffgets_com_rtu,        //  gets_com
f_UFG_rd,            //  answ_com
f_UFG_rd,            //  answ_flt
"UFG",             //  name
0,                     //  n_dev номер устройства в группе однотипных устройств
&UFG[0],            //  *ICP_dd
0,                     //  текущая функция
100,                   //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
//--------------------------
// device 2
//--------------------------
0,                     //  состояние запроса
ffgets_com_rtu,        //  gets_com
f_UFG_rd,            //  answ_com
f_UFG_rd,            //  answ_flt
"UFG/2",             //  name
1,                     //  n_dev номер устройства в группе однотипных устройств
&UFG[1],            //  *ICP_dd
0,                     //  текущая функция
100,                   //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
};
/*----------------------------*/

unsigned char Density_RD[] ={1,3,0x00,0xA3,0x00,0x02};  //seq = 0;
unsigned char Temperature_RD[] ={1,3,0x00,0x04,0x00,0x02};  //seq = 1;
unsigned char Volume_flow_RD[] ={1,3,0x00,0x02,0x00,0x02};  //seq = 2;

int seq=0;


void f_UFG(int ii)
{
  
int i;
char buf_tmp[20];
int i1,i2;

 if(UFG[ii].status == 0) return;

 if( f_timer(UFG[ii].time_stamp_pool,UFG[ii].pool_time ))
 {
  if((UFG_Rqst[ii].status == Req_Flt) ||
     (UFG_Rqst[ii].status == Req_OK)) goto m1;
  else if( f_timer(UFG[ii].time_stamp_pool,(UFG[ii].pool_time+UFG[ii].pool_time) ))
           goto m1;
   return;
m1:
        i1=6;
        switch (seq)
        {
          case 0:
            _fmemcpy(buf_tmp,Density_RD,(long int) 6);
          break;
          case 1:
            _fmemcpy(buf_tmp,Temperature_RD,(long int) 6);
          break;
          case 2:
            _fmemcpy(buf_tmp,Volume_flow_RD,(long int) 6);
          break;
        }

           buf_tmp[0]=UFG[ii].addr;
           i2=i1;
           f_prepareRTU(buf_tmp,&i2);
           _fmemcpy(UFG_Rqst[ii].Cmd,buf_tmp,(long)i2);
           UFG_Rqst[ii].cmd_lgth=i2;
           UFG_Rqst[ii].time_stamp=TimeStamp;
           UFG[ii].time_stamp_pool=TimeStamp;
           UFG[ii].n_transaction++;
           f_queue_put(UFG[ii].port, &UFG_Rqst[ii]);
 }
}
/*----------------------------*/

float var_ufg=0.0;
unsigned int UFG_inp_err[max_ufg]={0,0};

void f_UFG_rd(int ii)
{
// ii - номер порта
//
 struct COM_rqst *request;
 int i,nn,itmp;

 int count;

 count=n_bcom[ii];  // длина команды
 request=COM_que[ii].request[COM_que[ii].empt_ptr];
 nn=request->n_dev; // номер устройства OFP (0,1..)

 if(count < 3)
   {
      goto m_tout_err;
   }
 if(f_check_crc_RTU(cb_COM[ii],count)==0)
   {
      goto m_crc_err;
   }
 if(cb_COM[ii][1] & 0x80)
   {
//     OFP[nn].exc=cb_COM[ii][2];
//     OFP_fn[nn]=F_FAULT ;
     goto m_end;
   }

   if(count < 5)
   {
       goto m_tout_err;
   }

  var_ufg = f_get_float( &cb_COM[ii][0], 0);
  
  switch (seq)
  {
    case 0:
      s_UFG.dens = var_ufg;          //плотность
    break;
    case 1:
      s_UFG.temp = var_ufg;          //температура
    break;
    case 2:
      s_UFG.vol = var_ufg;          //объемный рабочий расход
    break;
  }
  seq++;
  if (seq > 2) seq = 0;

//  INP_OFP[nn]= f_get_int( &cb_COM[ii][0]); ;

 m_end:
  UFG[nn].n_success++;
  UFG_inp_err[nn]=0;
  f_queue_free(ii,&UFG_Rqst[nn]);
  return;

m_crc_err:
 UFG[nn].time_stamp_rcv=0;
UFG[nn].n_CRC_error++;
 goto m_err;

m_tout_err:
 UFG[nn].time_stamp_rcv=0;
 UFG[nn].n_timeout_error++;

m_err:
 UFG_inp_err[nn]++;

 if(UFG_inp_err[nn]>=UFG_max_inp_err)
 {
   UFG_inp_err[nn]=UFG_max_inp_err;
   f_icp_error(&UFG[nn],RD_ERR );
 }
 f_queue_free(ii,&UFG_Rqst[nn]);
}

//-----------------------------------------
float f_get_float( char *buf, int cod)
{
 union  { float f; unsigned long l; } o;

     switch(cod)  // ( Reg 0521 )
     {
     case 0:
    //  code == 0 : 0  1  2  3
    //             [3][4][5][6]
     o.l=
         ( (unsigned long)buf[6]  &0xff) |                // 3 l LSByte
         ( ((unsigned long)buf[5] << 8) &0xff00) |        // 2
         ( ((unsigned long)buf[4] << 16) &0xff0000)  |    // 1
         ( ((unsigned long)buf[3] << 24) &0xff000000);    // 0 l MSByte
        break;
     case 1:
    //  code == 1 : 2  3  0  1
    //             [3][4][5][6]
     o.l=
         (  (unsigned long)buf[4]  &  0x000000ff) |        // 3 l LSByte
         ( ((unsigned long)buf[3] << 8) &  0x0000ff00) |   // 2
         ( ((unsigned long)buf[6] << 16) & 0x00ff0000)  |  // 1
         ( ((unsigned long)buf[5] << 24) & 0xff000000);    // 0 l MSByte
         break;
      case 2:
    //  code == 2 : 1  0  3  2
    //             [3][4][5][6]

     o.l=
         ( (unsigned long)buf[5]  &0xff) |              // 3 l LSByte
         ( ((unsigned long)buf[6] << 8) &0xff00) |      // 2
         ( ((unsigned long)buf[3] << 16) &0xff0000)  |  // 1
         ( ((unsigned long)buf[4] << 24) &0xff000000);  // 0 l MSByte
         break;
      case 3:
    //  code == 3 : 3  2  1  0
    //             [3][4][5][6]
     o.l=( (unsigned long)(buf[3]) & 0x000000ff) |      // 3 l LSByte
         ( ((unsigned long)buf[4] << 8) &0xff00) |      // 2
         ( ((unsigned long)buf[5] << 16) &0xff0000)  |  // 1
         ( ((unsigned long)buf[6] << 24) &0xff000000);  // 0 l MSByte
       break;
       default : return 0.11;
     }

  if( o.l == 0x7fa00000)   // || ( o.l == 0x00007fa0))  RFT9739
     o.f= BIG_P;
  return o.f;

}
//-----------------------------------------
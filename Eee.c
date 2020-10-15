
char buf_eep[max_buf_eep];
int eee_num=0;
unsigned long int Eeprom_summ;
unsigned int ee_lgth_byte=0;
float fict_ftmp=0;
int fict_itmp=0;
int fict_sv=0;
int fict_int1=1;

// Список параметров сохраняемых в EEPROM
struct eee_dat EE_Dat[]=
{
&ComBaud[0],sizeof(ComBaud[0]),
&ComBaud[1],sizeof(ComBaud[1]),
&ComBaud[2],sizeof(ComBaud[2]),
&ComBaud[3],sizeof(ComBaud[3]),
&ComBaud[4],sizeof(ComBaud[4]),

&eee_num,sizeof(eee_num),

#if defined(ICP_7017C)

&I7017C[0].status         ,sizeof(I7017C[0].status        ),
&I7017C[0].port           ,sizeof(I7017C[0].port          ),
&I7017C[0].addr           ,sizeof(I7017C[0].addr          ),
&I7017C[0].type           ,sizeof(I7017C[0].type          ),
&I7017C[0].format         ,sizeof(I7017C[0].format        ),
&I7017C[0].channels       ,sizeof(I7017C[0].channels      ),
&I7017C[0].CRC_flag       ,sizeof(I7017C[0].CRC_flag      ),
&I7017C[0].pool_time      ,sizeof(I7017C[0].pool_time     ),
&I7017_Rqst[0].timeout    ,sizeof(I7017_Rqst[0].timeout   ),

&ICP_fict.status         ,sizeof(ICP_fict.status        ),
&ICP_fict.port           ,sizeof(ICP_fict.port          ),
&ICP_fict.addr           ,sizeof(ICP_fict.addr          ),
&ICP_fict.type           ,sizeof(ICP_fict.type          ),
&ICP_fict.format         ,sizeof(ICP_fict.format        ),
&ICP_fict.channels       ,sizeof(ICP_fict.channels      ),
&ICP_fict.CRC_flag       ,sizeof(ICP_fict.CRC_flag      ),
&ICP_fict.pool_time      ,sizeof(ICP_fict.pool_time     ),
&Rqst_fict.timeout    ,sizeof(I7017_Rqst[0].timeout   ),
#else

&ICP_fict.status         ,sizeof(ICP_fict.status        ),
&ICP_fict.port           ,sizeof(ICP_fict.port          ),
&ICP_fict.addr           ,sizeof(ICP_fict.addr          ),
&ICP_fict.type           ,sizeof(ICP_fict.type          ),
&ICP_fict.format         ,sizeof(ICP_fict.format        ),
&ICP_fict.channels       ,sizeof(ICP_fict.channels      ),
&ICP_fict.CRC_flag       ,sizeof(ICP_fict.CRC_flag      ),
&ICP_fict.pool_time      ,sizeof(ICP_fict.pool_time     ),
&Rqst_fict.timeout    ,sizeof(I7017_Rqst[0].timeout   ),

&ICP_fict.status         ,sizeof(ICP_fict.status        ),
&ICP_fict.port           ,sizeof(ICP_fict.port          ),
&ICP_fict.addr           ,sizeof(ICP_fict.addr          ),
&ICP_fict.type           ,sizeof(ICP_fict.type          ),
&ICP_fict.format         ,sizeof(ICP_fict.format        ),
&ICP_fict.channels       ,sizeof(ICP_fict.channels      ),
&ICP_fict.CRC_flag       ,sizeof(ICP_fict.CRC_flag      ),
&ICP_fict.pool_time      ,sizeof(ICP_fict.pool_time     ),
&Rqst_fict.timeout    ,sizeof(I7017_Rqst[0].timeout   ),

#endif

&MMI.status         ,sizeof(MMI.status        ),
&MMI.port           ,sizeof(MMI.port          ),
&MMI.addr           ,sizeof(MMI.addr          ),
&MMI.type           ,sizeof(MMI.type          ),
&MMI.format         ,sizeof(MMI.format        ),
&MMI.channels       ,sizeof(MMI.channels      ),
&MMI.CRC_flag       ,sizeof(MMI.CRC_flag      ),
&MMI.pool_time      ,sizeof(MMI.pool_time     ),
&MMI_Rqst.timeout    ,sizeof(MMI_Rqst.timeout   ),

&ComPortEcho          ,sizeof(ComPortEcho        ),
&ToutLicpScan         ,sizeof(ToutLicpScan       ),
&licp_amax            ,sizeof(licp_amax          ),
&LICP_Rqst.CRC_flag   ,sizeof(LICP_Rqst.CRC_flag ),

#if defined(ICP_7060) //(1) 

&ICP_fict.status         ,sizeof(ICP_fict.status        ),
&ICP_fict.port           ,sizeof(ICP_fict.port          ),
&ICP_fict.addr           ,sizeof(ICP_fict.addr          ),
&ICP_fict.type           ,sizeof(ICP_fict.type          ),
&ICP_fict.format         ,sizeof(ICP_fict.format        ),
&ICP_fict.channels       ,sizeof(ICP_fict.channels      ),
&ICP_fict.CRC_flag       ,sizeof(ICP_fict.CRC_flag      ),
&ICP_fict.pool_time      ,sizeof(ICP_fict.pool_time     ),
&Rqst_fict.timeout    ,sizeof(I7017_Rqst[0].timeout   ),

#else

&ICP_fict.status         ,sizeof(ICP_fict.status        ),
&ICP_fict.port           ,sizeof(ICP_fict.port          ),
&ICP_fict.addr           ,sizeof(ICP_fict.addr          ),
&ICP_fict.type           ,sizeof(ICP_fict.type          ),
&ICP_fict.format         ,sizeof(ICP_fict.format        ),
&ICP_fict.channels       ,sizeof(ICP_fict.channels      ),
&ICP_fict.CRC_flag       ,sizeof(ICP_fict.CRC_flag      ),
&ICP_fict.pool_time      ,sizeof(ICP_fict.pool_time     ),
&Rqst_fict.timeout    ,sizeof(I7017_Rqst[0].timeout   ),

#endif

#if defined(ICP_7041)

&ICP_fict.status         ,sizeof(ICP_fict.status        ),
&ICP_fict.port           ,sizeof(ICP_fict.port          ),
&ICP_fict.addr           ,sizeof(ICP_fict.addr          ),
&ICP_fict.type           ,sizeof(ICP_fict.type          ),
&ICP_fict.format         ,sizeof(ICP_fict.format        ),
&ICP_fict.channels       ,sizeof(ICP_fict.channels      ),
&ICP_fict.CRC_flag       ,sizeof(ICP_fict.CRC_flag      ),
&ICP_fict.pool_time      ,sizeof(ICP_fict.pool_time     ),
&Rqst_fict.timeout    ,sizeof(I7017_Rqst[0].timeout   ),
#else

&ICP_fict.status         ,sizeof(ICP_fict.status        ),
&ICP_fict.port           ,sizeof(ICP_fict.port          ),
&ICP_fict.addr           ,sizeof(ICP_fict.addr          ),
&ICP_fict.type           ,sizeof(ICP_fict.type          ),
&ICP_fict.format         ,sizeof(ICP_fict.format        ),
&ICP_fict.channels       ,sizeof(ICP_fict.channels      ),
&ICP_fict.CRC_flag       ,sizeof(ICP_fict.CRC_flag      ),
&ICP_fict.pool_time      ,sizeof(ICP_fict.pool_time     ),
&Rqst_fict.timeout    ,sizeof(I7017_Rqst[0].timeout   ),

#endif

&WDOG[0].status         ,sizeof( WDOG[0].status        ),
&WDOG[0].port           ,sizeof( WDOG[0].port          ),
&WDOG[0].CRC_flag       ,sizeof( WDOG[0].CRC_flag      ),
&WDOG[0].pool_time      ,sizeof( WDOG[0].pool_time     ),
&WDOG_Rqst[0].timeout   ,sizeof( WDOG_Rqst[0].timeout   ),

&WDOG_Rqst[1].timeout   ,sizeof( WDOG_Rqst[0].timeout   ),

&ComProt[0]     ,sizeof( ComProt[0] ),
&ComProt[1]     ,sizeof( ComProt[1] ),
&ComProt[2]     ,sizeof( ComProt[2] ),
&ComProt[3]     ,sizeof( ComProt[3] ),
&ComProt[4]     ,sizeof( ComProt[4] ),


#if defined(RTU_ComSK)

&ComSK[0].status         ,sizeof(ComSK[0].status        ),
&ComSK[0].port           ,sizeof(ComSK[0].port          ),
&ComSK[0].addr           ,sizeof(ComSK[0].addr          ),
&ComSK[0].pool_time      ,sizeof(ComSK[0].pool_time     ),
&ComSK_Rqst[0].timeout   ,sizeof(ComSK_Rqst[0].timeout   ),
&ComSK_WD_Rqst[0].timeout, sizeof(ComSK_WD_Rqst[0].timeout),
&sk_pool_period[0] ,sizeof(sk_pool_period[0]),

&ComSK[1].status         ,sizeof(ComSK[1].status        ),
&ComSK[1].port           ,sizeof(ComSK[1].port          ),
&ComSK[1].addr           ,sizeof(ComSK[1].addr          ),
&ComSK[1].pool_time      ,sizeof(ComSK[1].pool_time     ),
&ComSK_Rqst[1].timeout   ,sizeof(ComSK_Rqst[1].timeout   ),
&ComSK_WD_Rqst[1].timeout, sizeof(ComSK_WD_Rqst[1].timeout),
&sk_pool_period[1] ,sizeof(sk_pool_period[1]),

#else

&ICP_fict.status         ,sizeof(ICP_fict.status        ),
&ICP_fict.port           ,sizeof(ICP_fict.port          ),
&ICP_fict.addr           ,sizeof(ICP_fict.addr          ),
&ICP_fict.pool_time      ,sizeof(ICP_fict.pool_time     ),
&Rqst_fict.timeout    ,sizeof(Rqst_fict.timeout   ),
&fict_long                ,sizeof(fict_long),
&fict_long                ,sizeof(fict_long),

&ICP_fict.status         ,sizeof(ICP_fict.status        ),
&ICP_fict.port           ,sizeof(ICP_fict.port          ),
&ICP_fict.addr           ,sizeof(ICP_fict.addr          ),
&ICP_fict.pool_time      ,sizeof(ICP_fict.pool_time     ),
&Rqst_fict.timeout    ,sizeof(Rqst_fict.timeout   ),
&fict_long            ,sizeof(fict_long),
&fict_long            ,sizeof(fict_long),
#endif

#if ( defined(RTU_Delta) || defined(RTU_Delta_C))


&Delta[0].status         ,sizeof(Delta[0].status        ),
&Delta[0].port           ,sizeof(Delta[0].port          ),
&Delta[0].addr           ,sizeof(Delta[0].addr          ),
&Delta[0].pool_time      ,sizeof(Delta[0].pool_time     ),
&Delta_Rqst[0].timeout   ,sizeof(Delta_Rqst[0].timeout   ),
&fict_long                ,sizeof(fict_long),
&fict_long                ,sizeof(fict_long),
//&Delta_WD_Rqst[0].timeout, sizeof(Delta_WD_Rqst[0].timeout),
//&Dlt_pool_period[0] ,sizeof(sk_pool_period[0]),

&Delta[1].status         ,sizeof(Delta[1].status        ),
&Delta[1].port           ,sizeof(Delta[1].port          ),
&Delta[1].addr           ,sizeof(Delta[1].addr          ),
&Delta[1].pool_time      ,sizeof(Delta[1].pool_time     ),
&Delta_Rqst[1].timeout   ,sizeof(Delta_Rqst[1].timeout   ),
&fict_long                ,sizeof(fict_long),
&fict_long                ,sizeof(fict_long),
//&Delta_WD_Rqst[1].timeout, sizeof(Delta_WD_Rqst[1].timeout),
//&Dlt_pool_period[1] ,sizeof(sk_pool_period[1]),

#else

&ICP_fict.status         ,sizeof(ICP_fict.status        ),
&ICP_fict.port           ,sizeof(ICP_fict.port          ),
&ICP_fict.addr           ,sizeof(ICP_fict.addr          ),
&ICP_fict.pool_time      ,sizeof(ICP_fict.pool_time     ),
&Rqst_fict.timeout    ,sizeof(Rqst_fict.timeout   ),
&fict_long                ,sizeof(fict_long),
&fict_long                ,sizeof(fict_long),

&ICP_fict.status         ,sizeof(ICP_fict.status        ),
&ICP_fict.port           ,sizeof(ICP_fict.port          ),
&ICP_fict.addr           ,sizeof(ICP_fict.addr          ),
&ICP_fict.pool_time      ,sizeof(ICP_fict.pool_time     ),
&Rqst_fict.timeout    ,sizeof(Rqst_fict.timeout   ),
&fict_long            ,sizeof(fict_long),
&fict_long            ,sizeof(fict_long),

#endif
//-----------------------

&Com_lgth[0]   ,sizeof(Com_lgth[0]),
&Com_lgth[1]   ,sizeof(Com_lgth[0]),
&Com_lgth[2]   ,sizeof(Com_lgth[0]),
&Com_lgth[3]   ,sizeof(Com_lgth[0]),
&Com_lgth[4]   ,sizeof(Com_lgth[0]),
&Com_parity[0],sizeof(Com_parity[0]),
&Com_parity[1],sizeof(Com_parity[0]),
&Com_parity[2],sizeof(Com_parity[0]),
&Com_parity[3],sizeof(Com_parity[0]),
&Com_parity[4],sizeof(Com_parity[0]),
&Com_stop[0]  ,sizeof(Com_stop[0]),
&Com_stop[1]  ,sizeof(Com_stop[0]),
&Com_stop[2]  ,sizeof(Com_stop[0]),
&Com_stop[3]  ,sizeof(Com_stop[0]),
&Com_stop[4]  ,sizeof(Com_stop[0]),

//-----------------------
#if defined(RTU_MVD) //(1)

&MVD[0].status         ,sizeof(MVD[0].status        ),
&MVD[0].port           ,sizeof(MVD[0].port          ),
&MVD[0].addr           ,sizeof(MVD[0].addr          ),
&MVD[0].pool_time      ,sizeof(MVD[0].pool_time     ),
&MVD_Rqst[0].timeout   ,sizeof(MVD_Rqst[0].timeout   ),

&MVD[1].status         ,sizeof(MVD[1].status        ),
&MVD[1].port           ,sizeof(MVD[1].port          ),
&MVD[1].addr           ,sizeof(MVD[1].addr          ),
&MVD[1].pool_time      ,sizeof(MVD[1].pool_time     ),
&MVD_Rqst[1].timeout   ,sizeof(MVD_Rqst[1].timeout   ),

#else

&ICP_fict.status         ,sizeof(ICP_fict.status        ),
&ICP_fict.port           ,sizeof(ICP_fict.port          ),
&ICP_fict.addr           ,sizeof(ICP_fict.addr          ),
&ICP_fict.pool_time      ,sizeof(ICP_fict.pool_time     ),
&Rqst_fict.timeout    ,sizeof(Rqst_fict.timeout   ),

&ICP_fict.status         ,sizeof(ICP_fict.status        ),
&ICP_fict.port           ,sizeof(ICP_fict.port          ),
&ICP_fict.addr           ,sizeof(ICP_fict.addr          ),
&ICP_fict.pool_time      ,sizeof(ICP_fict.pool_time     ),
&Rqst_fict.timeout    ,sizeof(Rqst_fict.timeout   ),

#endif
//-----------------------
&UFG[0].status         ,sizeof(UFG[0].status        ),
&UFG[0].port           ,sizeof(UFG[0].port          ),
&UFG[0].addr           ,sizeof(UFG[0].addr          ),
&UFG[0].pool_time      ,sizeof(UFG[0].pool_time     ),
&UFG_Rqst[0].timeout   ,sizeof(UFG_Rqst[0].timeout   ),

&UFG[1].status         ,sizeof(UFG[1].status        ),
&UFG[1].port           ,sizeof(UFG[1].port          ),
&UFG[1].addr           ,sizeof(UFG[1].addr          ),
&UFG[1].pool_time      ,sizeof(UFG[1].pool_time     ),
&UFG_Rqst[1].timeout   ,sizeof(UFG_Rqst[1].timeout   ),

//-----------------------

&byte_timout_rtu[0]   ,sizeof(byte_timout_rtu),
&scom_tim[0]   ,sizeof(scom_tim),

&fict_itmp   ,sizeof(fict_itmp  ),
//-----------------------

&FlagWinSum ,sizeof(FlagWinSum ),

//-----------------------
&fict_ftmp      ,sizeof(fict_ftmp      ),
&fict_ftmp      ,sizeof(fict_ftmp      ),
&fict_ftmp      ,sizeof(fict_ftmp      ),
&fict_ftmp      ,sizeof(fict_ftmp      ),
&fict_itmp      ,sizeof(fict_itmp      ),
&fict_ftmp      ,sizeof(fict_ftmp      ),


&Password       ,sizeof(Password       ),

&analog_num[0]      ,sizeof(analog_num[0]    ),
&analog_scale[0]    ,sizeof(analog_scale[0]  ),
&analog_offset[0]   ,sizeof(analog_offset[0] ),

&analog_num[1]      ,sizeof(analog_num[0]    ),
&analog_scale[1]    ,sizeof(analog_scale[0]  ),
&analog_offset[1]   ,sizeof(analog_offset[0] ),
//-----------------------

&Password_m   ,sizeof( Password_m ),
//&fict_ftmp,sizeof( fict_ftmp ),

&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),

//&val_et[0][0]  ,sizeof( val_et),

&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),
&fict_ftmp,sizeof( fict_ftmp ),

//&n_dat_et[0]   ,sizeof( n_dat_et),
&fict_itmp,sizeof( fict_itmp ),
&fict_itmp,sizeof( fict_itmp ),
//-----------------------
&ZeroPage  ,sizeof(ZeroPage  ),
//&cod_float  ,sizeof(cod_float ),
&fict_int1  ,sizeof(fict_int1 ),

&EmptPage ,sizeof(EmptPage),

#if defined(EKD_800)

&Ekd.status         ,sizeof(Ekd.status        ),
&Ekd.port           ,sizeof(Ekd.port          ),
&Ekd.pool_time      ,sizeof(Ekd.pool_time     ),
&Ekd_Rqst.timeout   ,sizeof(Ekd_Rqst.timeout   ),
#else

&ICP_fict.status         ,sizeof(ICP_fict.status        ),
&ICP_fict.port           ,sizeof(ICP_fict.port          ),
&ICP_fict.pool_time      ,sizeof(ICP_fict.pool_time     ),
&Rqst_fict.timeout    ,sizeof(Rqst_fict.timeout   ),

#endif

&Lvl_7017[0]  ,sizeof(Lvl_7017 ),

&trans_num,sizeof(trans_num),

&Eeprom_summ,sizeof(Eeprom_summ),

NULL,0,
};
//---------------------------
void f_ee_num_init()
{
int i;
   for( i=0;EE_Dat[i].ptr != 0;i++) {}
   eee_num=i;
}
//---------------------------
int ee_members=0;
int f_wrt_eee()
/* Store all variables in EEE  */
{
int i,i1,i2,j;
int val;
char *ch_ptr;

 Eeprom_summ=0;

        for(i=0,i1=0;EE_Dat[i].ptr!=0;i++)
        {
          i2=EE_Dat[i].l;
          ch_ptr=(char *)EE_Dat[i].ptr;
          for(j=0;j<i2;j++)
          {
            buf_eep[i1]=*ch_ptr++;
            if(EE_Dat[i].ptr!= &Eeprom_summ)
//              Eeprom_summ +=buf_eep[i1];
              Eeprom_summ +=((unsigned long)(buf_eep[i1])) & 0xff;
            i1++;
          }
        }

          i2=EE_Dat[i].l;
          ch_ptr=(char *)EE_Dat[i].ptr;

 EE_WriteEnable();
   i2= EE_MultiWrite_L(0,i1,buf_eep);
 EE_WriteProtect();

    ee_members=i;

// printf (" %d",i);
 return i2;
}
//--------------------------------------
int f_chk_EEE_CRC()
{
int i,i1,i2,j;
int val;
char *ch_ptr;
unsigned long int Ee_summ;

 Ee_summ=0;

 if((i1=EE_MultiRead_L(0,2048,buf_eep))<0 )
 {
   return i1;
 }
//-----------------------------
   // Check CRC
        for(i=0,i1=0;EE_Dat[i].ptr!=0;i++)
        {
          i2=EE_Dat[i].l;
          ch_ptr=(char *)EE_Dat[i].ptr;
          for(j=0;j<i2;j++)
          {
            if(EE_Dat[i].ptr==&Eeprom_summ)
              *ch_ptr++=buf_eep[i1++];
            else
              Ee_summ +=((unsigned long)(buf_eep[i1++])) & 0xff;
          }
        }

     ee_lgth_byte=i1;
//  printf("\n\rf_chk_EEE_CRC: %d записей,%dбайт,CRC_count=%08lx,CRC_rd%08lx\n\r",i,i1,Ee_summ,Eeprom_summ);
   if(Ee_summ != Eeprom_summ) return -100;
   return 0;
}
//--------------------------------------
int f_rd_eee()
/* Read all variables from  buf_eep  */
{
int i,i1,i2,j;
int val;
char *ch_ptr;

unsigned long int Ee_summ;
 Ee_summ=0;
//-----------------------------

   for(i=0,i1=0;(EE_Dat[i].ptr!=0)&&(i<(eee_num-1)) ;i++)
   {
     i2=EE_Dat[i].l;
     ch_ptr=(char *)EE_Dat[i].ptr;
     for(j=0;j<i2;j++)
     {
       *ch_ptr++=buf_eep[i1];
       Ee_summ +=buf_eep[i1++];
       if(eee_num== 398) eee_num=461;
     }
   }
   f_ee_num_init();

   return 0;
}
//--------------------------------------
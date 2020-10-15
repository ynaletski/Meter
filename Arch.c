
struct arch_str_t tim_000 = {
0,34,1,
30,7,9
};
//-------------------
struct arch_str_t ahour00=
{
0,       //   sec
0,       //   min
8,       //   hour
};
//-------------------
struct arch_str_t adate00=
{
0,       //   sec
0,       //   min
0,       //   hour
1,       //   day
1,       //   month
9,       //   year
};
//-------------------
//                   sec min hour
struct tm  mhour00 = { 0, 0,  8 };
//                sec min hour day   month-1, year-1900
struct tm  mdat0= { 0, 34, 01,   30,     6,   109 };
struct tm  mdat = { 0, 0, 12,   25,     11,    88 };
time_t ltime0,ltime;
//----------------------------
//-------------------------------
void f_prepare_t(struct arch_str_t *s_arch)
{
  TIME_DATE tim_dat;
 GetTimeDate(&tim_dat);
 s_arch->year  = tim_dat.year -2000 ;
 s_arch->month = tim_dat.month ;
 s_arch->day   = tim_dat.day   ;
 s_arch->hour  = tim_dat.hour  ;
 s_arch->min   = tim_dat.minute;
 s_arch->sec   = tim_dat.sec   ;
}

//-------------------------------
#define UFG_max_inp_err 10

void f_UFG(int ii);
void f_UFG_rd(int ii);
void f_UFG_rd_t(int ii);

extern struct s_icp_dev UFG[];
extern struct COM_rqst UFG_Rqst[];

float f_get_float( char *buf, int cod);

struct s_UFG_dev
{
   float dens;
   float temp;
   float vol;
};
extern struct s_UFG_dev s_UFG;
extern int seq;
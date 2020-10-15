char *trans[]=
{ 
    "",
    "Плотность",
    "Температура",
    "Давление",
    "Turbo Flow UFG",
};

int max_count_trans=(sizeof(trans)/sizeof(trans[0])) -1;

int Ytrans = 2; //положение курсора > на MMI
int max_count_tmp=0;
int trans_num = 1; //номер выбранного лотка

float level;
typedef struct agrs
{
    double *a;
    double *b;
    double *x;
    int n;
    int i;
    int num_process;
    int total_processes;
    int error;
}args;

void sinhr (int p,  double *a = 0, int *max_i = 0, int *max_j = 0);

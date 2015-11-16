#include <pthread.h>

#include "synchronize.h"

void sinhr (int p, double *a, int *max_i, int *max_j)
{
    static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
    static pthread_cond_t c_in = PTHREAD_COND_INITIALIZER;
    static pthread_cond_t c_out = PTHREAD_COND_INITIALIZER;
    static int t_in = 0;
    static int t_out = 0;
    static double *pa = 0;
    static int *pmax_i = 0;
    static int *pmax_j = 0;

    int i;
    if (p <= 1)
        return;
    pthread_mutex_lock (&m);
    if (!pa)
    {    pa = a; pmax_i = max_i; pmax_j = max_j;}
    else
    {
        if (*pa < *a)
        {   *pa = *a; *pmax_i = *max_i; *pmax_j = *max_j;}
    }
    t_in++;
    if (t_in >= p)
    {
        t_out = 0;
        pthread_cond_broadcast (&c_in);
    }
    else
    {
        while (t_in < p)
            pthread_cond_wait (&c_in, &m);
    }
    if (pa != a)
    {
        *a = *pa; *max_i = *pmax_i; *max_j = *pmax_j;
    }
    t_out++;
    if (t_out >= p)
    {
        pa = 0; pmax_i = 0; pmax_j = 0;
        t_in = 0;
        pthread_cond_broadcast (&c_out);
    }
    else
    {
        while (t_out < p)
            pthread_cond_wait (&c_out, &m);
    }
    pthread_mutex_unlock (&m);
}

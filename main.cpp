#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#include "matrix.h"
#include "synchronize.h"
#include "get_time.h"
#include "processes.h"


#define MAX_N 6

int main(int argc, char * argv[])
{
    pthread_t *tid;//number of thread
    args *arg;
    int total_processes;
    double *a, *b, *x;
    int res1, res2;
    long int t;
    int n;
    int N;
    int i;
    char * filename = 0;
    const char * name = "c.txt";
    if( argc != 3 && argc != 4 )
    {
        printf("Usage : %s <n> <total_processes> <filename>\n", argv[0]);
        return 0;
    }
    n = atoi(argv[1]);//from number to string
    total_processes = atoi (argv[2]);
    if(!n || !total_processes)
    {
        printf("Usage : %s <n> <total_processes> <filename>\n", argv[0]);
        return 0;
    }
    a = new double[n*n];
    b = new double[n];
    x = new double[n];
    tid = new pthread_t[total_processes];
    arg = new args[total_processes];
    if(argc > 3)
        filename = argv[3];

    if(filename)
    {
        res1 = read_matrix(a, n, "a.txt");
        res2 = read_vector(b, n, "b.txt");
        if(res1 || res2)
        {
            printf("cannot read from file\n");
            delete [] tid;
            delete [] arg;
            delete [] a;
            delete [] b;
            delete [] x;
            return 1;
        }
    }
    else
    {
        init_matrix(a, n);
        init_vector(b, a, n);
    }
    printf("matrix A:\n");
    print_matrix(a, n);
    printf("vector b:\n");
    print_vector(b, n);

    for (i = 0; i < total_processes; i++)
    {
        arg[i].a = a;
        arg[i].b = b;
        arg[i].n = n;
        arg[i].total_processes = total_processes;
        arg[i].num_process = i;
        arg[i].error = 0;
    }

    t = get_full_time ();
    for (i = 0; i < total_processes; i++)
    {
        if (pthread_create (tid + i, 0, &thread_method_of_reflections, arg + i))
        {
            printf ("Cannot create thread %d\n", i);
            return 2;
        }
    }
    for (i = 0; i < total_processes; i++)
        pthread_join (tid[i], 0);
    back_hod(a, b, x, n);
    t = get_full_time () - t;
    N = (n < MAX_N) ? n : MAX_N;
    printf("result : ");
    for(i = 0; i < N; i++)
        printf("%lg ", x[i]);
	printvectorfile(x,n,name);
     if(filename)
    {
        read_matrix(a, n, "a.txt");
        read_vector(b, n, "b.txt");
        printf("\nResidual = %le\nElapsed time = %Lg\n",SolutionError(n,a,b,x),(long double)t/(CLOCKS_PER_SEC));
    }
    else
    {
        init_matrix(a, n);
        init_vector(b, a, n);
        printf("\nResidual = %le\nError = %le\nElapsed time = %Lg\n",SolutionError(n,a,b,x), SolutionAccuracy(n,x),(long double)t/(CLOCKS_PER_SEC));
    }
    delete [] tid;
    delete [] arg;
    delete [] a;
    delete [] b;
    delete [] x;
    return 0;
}

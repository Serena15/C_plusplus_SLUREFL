#include <stdio.h>
#include <math.h>

#include "matrix.h"
#define EPS 1e-40
#define MAX_N 6

double f(int i, int j)
{
   //return 1./(i + j + 1);
    return ((i > j) ? i : j) + 1;
}

void init_matrix (double *a, int n)
{
    int i, j;
    for(i = 0; i < n; i++)
        for(j = 0; j < n; j++)
            a[i*n + j] = f(i, j);
}
void init_vector (double *b, double *a, int n)
{
    int i, j;
    for(i = 0; i < n; i++)
    {
        b[i] = 0;
        for(j = 0; j < n; j += 2)
            b[i] += a[i*n + j];
    }
}

int read_matrix (double *a, int n, const char *filename)
{
    FILE *fin;
    if(!(fin = fopen(filename, "r")))
        return -1;
    int i, j;
    for(i = 0; i < n; i++)
        for(j = 0; j < n; j++)
            if(fscanf(fin, "%lf", &a[i*n + j]) != 1)
            {   fclose(fin);return -2;}
    return 0;
}

int read_vector(double *b, int n, const char *filename)
{
    FILE *fin;
    if(!(fin = fopen(filename, "r")))
        return -1;
    int i;
    for(i = 0; i < n; i++)
        if(fscanf(fin, "%lf", &b[i]) != 1)
        {   fclose(fin);return -2;}
    return 0;

}

void print_matrix (double *a, int n)
{
    int i, j;
    int N = (n < MAX_N) ? n : MAX_N;
    for(i = 0; i < N; i++)
    {
        for(j = 0; j < N; j++)
            printf("%13.6lf ", a[i*n + j]);
        printf("\n");
    }
}

void print_vector (double *b, int n)
{
    int i;
    int N = (n < MAX_N) ? n : MAX_N;
    for(i = 0; i < N; i++)
        printf("%13.6lg ", b[i]);
    printf("\n");
}
void printvectorfile (double *b, int n,const char *name)
{
    int i;
    FILE *out;
    if(!(out=fopen(name,"w"))){printf("Can not open file w\n");return;}
    for(i = 0; i < n; i++){
        fprintf(out,"%lg \n", b[i]);}
    fclose(out);
}

void back_hod(double *a, double *b, double *x, int n)
{
    int i, j;
    for(i = n - 1; i >= 0; i--)
    {
        x[i] = b[i];
        for(j = i + 1; j < n; j++)
            x[i] -= a[i*n + j] * x[j];
    }
}

double SolutionError(int n, double* a, double* b, double* x)
{
	int i;
	int j;
	double tmp;
	double rezult;

	rezult = 0.0;
	for (i = 0; i < n; ++i)
	{
		tmp = 0.0;
		for (j = 0; j < n; ++j)
			tmp += a[i * n + j] * x[j];
		tmp -= b[i];

		rezult += tmp * tmp;
	}

	return sqrt(rezult);
}

double SolutionAccuracy(int n, double* x)
{
	int i;
	double tmp;
	double rezult;

	rezult = 0.0;
	for (i = 0; i < n; ++i)
	{
		tmp = x[i];

		if (i % 2 == 0)
			tmp -= 1.0;

		rezult += tmp * tmp;
	}

	return sqrt(rezult);
}

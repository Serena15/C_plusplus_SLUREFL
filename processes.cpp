#include <math.h>
#include <stdio.h>

#include "processes.h"
#include "synchronize.h"
#include "matrix.h"
#include "get_time.h"

#define EPS 1e-40

void* thread_method_of_reflections (void *ptr)
{
	args* arg = (args*)ptr;
    	int i, j, k;
	double* x;
	int n = arg->n;
	x=new double[n];
    	double *a = arg->a;
    	double * b = arg->b;
    	int first;
    	int last;
    	int num_process = arg->num_process;//number current thread
    	int total_processes = arg->total_processes;//all threads
	double tmp,norm,norm_min,sc_pr;
	for (i=0;i<n;i++){
		first = (n - i) * num_process;
        	first = first / total_processes + i;
        	last = (n - i) * (num_process + 1);
        	last = last / total_processes + i;
		//division processes half
        	tmp=0;
		for (j=i+1;j<n;j++){
			tmp+=a[j*n+i]*a[j*n+i];
		}
		norm=sqrt(a[i*n+i]*a[i*n+i]+tmp);
		norm_min=sqrt((a[i*n+i]-norm)*(a[i*n+i]-norm)+tmp);
			if (norm<EPS){
				printf("error norm\n");
				return 0;
			}
		if (norm_min==0){
			for (j=0;j<n;j++){
				x[j]=0;
			}		
		}
		else{
			for (j=i+1;j<n;j++){
				x[j]=a[j*n+i]/norm_min;
			}
			x[i]=(a[i*n+i]-norm)/norm_min;
		}
		sinhr(total_processes);
		//get vector of reflection
		for (j=first;j<last;j++){
			sc_pr=0;
			for (k=i;k<n;k++){
				sc_pr+=x[k]*a[k*n+j];			
			}
			for (k=i;k<n;k++){
				a[k*n+j]-=2*x[k]*sc_pr;
			}
		}
		sinhr(total_processes);
		//multiply matrix A by matrix of reflection
		if (num_process == 0)
       		 {			
			sc_pr=0;
			for (k=i;k<n;k++){
				sc_pr+=x[k]*b[k];			
			}
			for (k=i;k<n;k++){
				b[k]-=2*x[k]*sc_pr;
			}
			//and vector of values multiply by matrix of reflection
			for (j=i+1;j<n;j++){
				a[i*n+j]/=a[i*n+i];		
			}
			b[i]/=a[i*n+i];
			a[i*n+i]=1;	
			//normirovka
		}
		sinhr(total_processes);
	}
	delete [] x;
	return 0;
}

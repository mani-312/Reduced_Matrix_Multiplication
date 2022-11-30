// thread0.c
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <chrono>
#include <fstream>
#include <assert.h>

using namespace std;

#define TIME_NOW std::chrono::high_resolution_clock::now()
#define TIME_DIFF(gran, start, end) std::chrono::duration_cast<gran>(end - start).count()
int *A;
int *B;
int *C;
int NG;
struct arg{
	int ii;
	int jj;
};
void eval_row(int i)
{
    int rowA = i<<1;
	int N = NG;
	int n = N>>1;
	int b = 4,sum;
	for(int rowB = 0; rowB < N; rowB += b){
	      for(int k = 0; k < N; k+=1) 
	      {
		sum = A[rowA * N + rowB] * B[rowB * N + k];
		sum += A[(rowA+1) * N + rowB] * B[rowB * N + k];
		sum += A[rowA * N + rowB+1] * B[(rowB+1) * N + k];
		sum += A[(rowA+1) * N + rowB+1] * B[(rowB+1) * N + k];
		sum += A[rowA * N + rowB+2] * B[(rowB+2) * N + k];
		sum += A[(rowA+1) * N + rowB+2] * B[(rowB+2) * N + k];
		sum += A[rowA * N + rowB+3] * B[(rowB+3) * N + k];
		sum += A[(rowA+1) * N + rowB+3] * B[(rowB+3) * N + k];

		C[i*n+(k>>1)]+=sum;
	       
	      }
    	}
}
void * helper6(void *a)
{
	struct arg *p = (struct arg*)a;
	int i = p->ii;
	int j = p->jj;
	while(i<j)
		eval_row(i++);
	
	pthread_exit(NULL);
}
void init(int N,int *matA,int *matB,int *output)
{
	A = (int*)malloc(sizeof(int*)*(N*N));
	A = matA;
	B = (int*)malloc(sizeof(int*)*(N*N));
	B = matB;
	C = (int*)malloc(sizeof(int*)*((N>>1)*(N>>1)));
	C = output;
}
void multiThread(int N, int *matA, int *matB, int *output)
{
    int n = N>>1;
    NG = N;
    
    int n_threads = 16;
    int each = n/n_threads;
    
    init(N,matA,matB,output);
    pthread_t thread[n_threads];
    struct arg arg_pool[n_threads];
    
    for(int i = 0;i<n_threads;i++)
    {
    	     arg_pool[i].ii = i*each;
    	     arg_pool[i].jj = (i+1)*each;
             pthread_create(&thread[i], NULL, helper6, &arg_pool[i]);
    }
    for(int i = 0;i<n_threads;i++)
    {
        	pthread_join(thread[i], NULL);
    }
}


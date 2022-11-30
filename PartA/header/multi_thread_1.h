
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <chrono>
#include <fstream>
#include <smmintrin.h>
#include <immintrin.h>

#include <assert.h>

using namespace std;

int *A;
int *B;
int *C;
int NG;
struct arg{
	int ii;
	int jj;
};

void eval(int i)
{
   
	int rowA = i<<1;
	int N = NG;
	int n = N>>1;
	int iter = 2;
	
	__m256i v1, v2, v3, v4,v5,v6; 
	int ind;
	while(iter--)
	{
		for(int rowB = 0;rowB<N;rowB++)
		{
			 v1 = _mm256_set1_epi32(A[rowA*N+rowB]);
			for(int k = 0;k<N;k+=8)
			{
				v2 = _mm256_loadu_si256((const __m256i*)&B[rowB*N+k]);
				v3 = _mm256_mullo_epi32(v1, v2);
				int temp[8];
				_mm256_storeu_si256((__m256i*)temp, v3);
				
				ind = (rowA>>1)*n+(k>>1); 
				C[ind]+=temp[0]+temp[1];
				C[ind+1]+=temp[2]+temp[3];
				C[ind+2]+=temp[5]+temp[4];
				C[ind+3]+=temp[7]+temp[6];
			}
			
		}
		      rowA++;
    	}
}
void * helper1(void *a)
{

         struct arg *p = (struct arg*)a;
	int i = p->ii;
        int j = p->jj;
        while(i<j)
        	eval(i++);
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
    init(N,matA,matB,output);
    
    int n_threads = 16;
    int each = n/n_threads;
    
    pthread_t thread[n_threads];
    struct arg arg_pool[n_threads];
    
    for(int i = 0;i<n_threads;i++)
    {
    	     arg_pool[i].ii = i*each;
    	     arg_pool[i].jj = (i+1)*each;
             pthread_create(&thread[i], NULL, helper1, &arg_pool[i]);
    }
    for(int i = 0;i<n_threads;i++)
    {
        	pthread_join(thread[i], NULL);
    }
}

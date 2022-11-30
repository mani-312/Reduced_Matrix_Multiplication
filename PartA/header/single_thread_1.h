#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <chrono>
#include <fstream>
#include <assert.h>
#include <smmintrin.h>
#include <immintrin.h>
#include <cstring>
using namespace std;

#define TIME_NOW std::chrono::high_resolution_clock::now()
#define TIME_DIFF(gran, start, end) std::chrono::duration_cast<gran>(end - start).count()



//m256
void single_thread(int N,int *A,int *B,int *C)
{
	__m256i v1, v2, v3, v4,v5,v6; 
	int n = N>>1,ind;
	for(int rowA = 0;rowA<N;rowA++)
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
	}
}


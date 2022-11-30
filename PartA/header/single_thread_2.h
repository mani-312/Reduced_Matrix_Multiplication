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
	int n = N>>1;
	for(int rowA = 0;rowA<N;rowA++)
	{
		for(int rowB = 0;rowB<N;rowB++)
		{
			 v1 = _mm256_set1_epi32(A[rowA*N+rowB]);
			for(int k = 0;k<N;k+=16)
			{
				v2 = _mm256_loadu_si256((const __m256i*)&B[rowB*N+k]);
				v3 = _mm256_mullo_epi32(v1, v2);
				int temp[16];
				_mm256_storeu_si256((__m256i*)temp, v3);
				
				v2 = _mm256_loadu_si256((const __m256i*)&B[rowB*N+(k+8)]);
				v3 = _mm256_mullo_epi32(v1, v2);
				_mm256_storeu_si256((__m256i*)(temp+8), v3);
				
				v4 = _mm256_set_epi32(temp[15]+temp[14], temp[13]+temp[12], temp[11]+temp[10], temp[9]+temp[8],temp[6]+temp[7], temp[4]+temp[5], temp[2]+temp[3], temp[0]+temp[1]);
				v5 = _mm256_loadu_si256((const __m256i*)&C[(rowA>>1)*n+(k>>1)]);
				v6 = _mm256_add_epi32 (v4, v5);
				_mm256_storeu_si256((__m256i*)&C[(rowA>>1)*n+(k>>1)], v6);
			}
			
		}
	}
}

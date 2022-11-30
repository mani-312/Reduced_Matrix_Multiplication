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


void single_thread(int N, int *matA, int *matB, int *output)
{
	int n = N>>1;
	int sum;
	int b = 4;
  for(int rowA = 0; rowA <N; rowA +=2) {
    for(int rowB = 0; rowB < N; rowB += b){
      for(int k = 0; k < N; k+=1) 
      {
      	
        sum = matA[rowA * N + rowB] * matB[rowB * N + k];
        sum += matA[(rowA+1) * N + rowB] * matB[rowB * N + k];
        sum += matA[rowA * N + rowB+1] * matB[(rowB+1) * N + k];
        sum += matA[(rowA+1) * N + rowB+1] * matB[(rowB+1) * N + k];
        sum += matA[rowA * N + rowB+2] * matB[(rowB+2) * N + k];
        sum += matA[(rowA+1) * N + rowB+2] * matB[(rowB+2) * N + k];
        sum += matA[rowA * N + rowB+3] * matB[(rowB+3) * N + k];
        sum += matA[(rowA+1) * N + rowB+3] * matB[(rowB+3) * N + k];
        
        output[(rowA>>1)*n+(k>>1)]+=sum;
       
      }
    }
  }
  
}


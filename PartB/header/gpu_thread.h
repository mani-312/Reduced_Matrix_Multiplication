#include <algorithm>
#include <cstdlib>
#include <iostream>

using namespace std;

__global__ void Red_matrixMul(const int *a, const int *b, int *c, int N) {

  int n = N>>1;
  int row = blockIdx.y * blockDim.y + threadIdx.y;
  int col = blockIdx.x * blockDim.x + threadIdx.x;

  int val = 0;

  for (int k = 0; k < N; k++) {
    val += a[(row<<1) * N + k] * b[k * N + (col<<1)];
    val += a[(row<<1) * N + k] * b[k * N + (col<<1)+1];
    val += a[((row<<1)+1) * N + k] * b[k * N + (col<<1)];
    val += a[((row<<1)+1) * N + k] * b[k * N + (col<<1)+1];
  }

  c[row * n + col] = val;
}
void gpuThread(int N, int *matA, int *matB, int *output)
{
    int n = N>>1;
    size_t mat_bytes = N * N * sizeof(int);
    size_t res_bytes = n * n * sizeof(int);
    int *A, *B, *C;
  cudaMalloc(&A, mat_bytes);
  cudaMalloc(&B, mat_bytes);
  cudaMalloc(&C, res_bytes);

  cudaMemcpy(A, matA, mat_bytes, cudaMemcpyHostToDevice);
  cudaMemcpy(B, matB, mat_bytes, cudaMemcpyHostToDevice);

  int THREADS = 16;

  int BLOCKS = n / THREADS;

  dim3 threads(THREADS, THREADS);
  dim3 blocks(BLOCKS, BLOCKS);

  Red_matrixMul<<<blocks, threads>>>(A, B, C, N);

  cudaMemcpy(output, C, res_bytes, cudaMemcpyDeviceToHost);

  cudaFree(A);
  cudaFree(B);
  cudaFree(C);

}

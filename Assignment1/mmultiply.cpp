#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sys/time.h>
#include <time.h>
#include "counters.h"

void opt_simd_sgemm(float *Y, float *A, float *B, int n);
void opt_scalar1_sgemm(float *Y, float *A, float *B, int n);
void opt_scalar0_sgemm(float *Y, float *A, float *B, int n);
void naive_sgemm(float *Y, float *A, float *B, int n);

double getTime()
{
	struct timeval tv;
	gettimeofday(&tv, 0);
	return tv.tv_sec + 1e-6 * tv.tv_usec;
}

int main(int argc, char *argv[])
{
  hwCounter_t c;
  c.init = false;
  initInsns(c);
  initTicks(c);

  int n = (1<<10);
  float* A = new float[n*n];
  float* B = new float[n*n];
  float* Y = new float[n*n];
  double flop = 3 * n * n;
  for(int x = 0; x < n*n; x++)
  {
  	A[x] = x;
  	B[x] = x;
  }

  double start = getTime();
  double begin = (double)getTicks(c);
  double count = (double)getInsns(c);
  naive_sgemm(Y, A, B, n);
  double executed = (double)getInsns(c) - count;
  double elapsed = (double)getTicks(c) - begin;
  double finish = getTime() - start;

  printf("naive_sgemm IPC: %f. Flops: %f.\n", executed/elapsed, flop/finish);

  start = getTime();
  count = (double)getInsns(c);
  begin = (double)getTicks(c);
  opt_simd_sgemm(Y, A, B, n);
  elapsed = (double)getTicks(c) - begin;
  executed = (double)getInsns(c) - count;
  finish = getTime() - start;

  printf("opt_simd_sgemm IPC: %f. Flops: %f.\n", executed/elapsed, flop/finish);

  start = getTime();
  count = (double)getInsns(c);
  begin = (double)getTicks(c);
  opt_scalar0_sgemm(Y, A, B, n);
  elapsed = (double)getTicks(c) - begin;
  executed = (double)getInsns(c) - count;
  finish = getTime() - start;

  printf("opt_scalar0_sgemm IPC: %f. Flops: %f.\n", executed/elapsed, flop/finish);

  start = getTime();
  count = (double)getInsns(c);
  begin = (double)getTicks(c);
  opt_scalar1_sgemm(Y, A, B, n);
  elapsed = (double)getTicks(c) - begin;
  executed = (double)getInsns(c) - count;
  finish = getTime() - start;

  printf("opt_scalar1_sgemm IPC: %f. Flops: %f.\n", executed/elapsed, flop/finish);
  
  delete [] A;
  delete [] B;
  delete [] Y;
}
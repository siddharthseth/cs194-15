#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <sys/time.h>
#include <time.h>
#include "counters.h"
#include "simd_copy.cpp"

double getAverageStep()
{
	//Initialize a hardware counter
	hwCounter_t c1;
	c1.init = false;
	initTicks(c1);
	srand(time(NULL));

	for(int i = 8000; i < 2000001; i += 199200){
		int* arr = (int *) malloc(sizeof(int) * i);

		for(int j = 0; j<i; j++)
		{
			arr[j] = rand() % i;
		}

		long long total = 0;
		int j = 0;
		uint64_t current_time = getTicks(c1);
		for(int m = 0; m < 1048576; m++){
			j = arr[j];
		}
		uint64_t elapsed = getTicks(c1) - current_time;
		printf("Length N = %d. Last index = %d\n", i*4, j);
		double average = elapsed/1048576.0;
		printf("Elapsed time in ticks %lu. Average step time in ticks %f\n\n", elapsed, average);
	}
}

double getTime()
{
	struct timeval tv;
	gettimeofday(&tv, 0);
	return tv.tv_sec + 1e-6 * tv.tv_usec;
}

double timeArrayCopy()
{
	for(int N = 8000; N < 2000001; N+=199200){
		int* source = (int *) malloc(sizeof(int) * N);
		//Create array of length N
		for(int i = 0; i< N; i++)
		{
			source[i] = i;
		}

		int* arrCpy = (int *) malloc(sizeof(int) * N);
		// warm up cache before timing
		for(int i = 0; i < 3; i++)
			memcpy(arrCpy, source, sizeof(int) * N);
		double before = getTime();
		memcpy(arrCpy, source, sizeof(int) * N);
		double elapsed = getTime() - before;
		double memcpy_speed = (sizeof(int) * N * 8.0) / (1000000.0 * elapsed);

		for(int i = 0; i < 3; i++)
			simd_memcpy(arrCpy, source, sizeof(int) * N);
		before = getTime();
		simd_memcpy(arrCpy, source, sizeof(int) * N);
		elapsed = getTime() - before;
		double simd_memcpy_speed = (sizeof(int) * N * 8.0) / (1000000.0 * elapsed);

		for(int i = 0; i < 3; i++)
			simd_memcpy_cache(arrCpy, source, sizeof(int) * N);
		before = getTime();
		simd_memcpy_cache(arrCpy, source, sizeof(int) * N);
		elapsed = getTime() - before;
		double simd_memcpy_cache_speed = (sizeof(int) * N * 8.0) / (1000000.0 * elapsed);
		
		delete source;
		delete arrCpy;
		printf("sizeof(ints) = %lu bytes, memcpy = %f, simd_memcpy = %f, simd_memcpy_cache = %f\n", sizeof(int) * N, memcpy_speed, simd_memcpy_speed, simd_memcpy_cache_speed);	
	}
}

int main(int argc, char *argv[])
{
	getAverageStep();
}
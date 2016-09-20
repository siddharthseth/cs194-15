#include <pthread.h>
#include <cassert>
#include <cstdio>

typedef struct
{
  double **a;
  double **b;
  double **c;
  int start;
  int end;
} worker_t;

void *matmuld_worker(void *arg)
{
  worker_t *t = static_cast<worker_t*>(arg);
  double **a = t->a;
  double **b = t->b;
  double **c = t->c;
  for(int i = t->start; i < t->end; i++)
    {
      for(int j = 0; j < 1024; j++)
	{
	  for(int k = 0; k < 1024; k++)
	    {
	      c[i][j] += a[i][k]*b[k][j];
	    }
	}
    }
}

void pthread_matmuld(double **a,
		     double **b,
		     double **c,
		     int nthr)
{
  /* CS194: use pthreads to launch 
   * matrix multply worker threads.
   *
   * The structure and worker function
   * are good hints...
   */
  pthread_t *thr = new pthread_t[nthr];
  worker_t *tInfo = new worker_t[nthr];
  int range = 1024/nthr;
  for(int i = 0; i < nthr; i++){
    tInfo[i].a = a;
    tInfo[i].b = b;
    tInfo[i].c = c;
    tInfo[i].start = i * range;
    if(i == nthr - 1)
      tInfo[i].end = 1024;
    else
      tInfo[i].end = i * range + range;
    //printf("Creating thread %d\n", i);
    int rc = pthread_create(&thr[i], NULL, matmuld_worker, (void*)&tInfo[i]);
    if(rc){
      printf("ERROR; return code from pthread_create() is %d\n", rc);
    }
  }

  void *status;
  for(int i = 0; i < nthr; i++){
    int rc = pthread_join(thr[i], &status);
    if(rc){
      printf("ERROR; return code from pthread_join() is %d\n", rc);
      break;
    }
    //printf("Main: completed join with thread %d having a status of %ld\n", i, (long) status);
  }

  delete [] thr;
  delete [] tInfo;
}

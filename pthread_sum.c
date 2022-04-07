#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

double *a=NULL, sum=0.0;
int    N, size;

typedef struct foo {
  pthread_t ptid;
  double *a, sum;
  int N, size;
  long tid;
} FOO;

void *compute(void *arg) {
  FOO *info = (FOO *)arg;
  int i;
  long tid = info->tid;

  int myN = info->N / info->size;
  int myStart = tid * myN;
  int myEnd = myStart + myN;
  if(tid == (info->size - 1))
    myEnd = info->N;
    
  // compute partial sum
  double mysum = 0.0;
  for (i=myStart; i<myEnd; i++)
    mysum += info->a[i];

  printf("Hello, I am thread %ld of %d\n", info->tid + 1, info->size);

  // grab the lock, update global sum, and release lock
  pthread_mutex_lock(&mutex);
  sum += mysum;
  pthread_mutex_unlock(&mutex);

  return (NULL);
}

int main(int argc, char **argv) {
    long i;
    FOO *info;

    if (argc != 3) {
       printf("Usage: %s <# of elements> <# of threads>\n",argv[0]);
       exit(-1);
    }

    N = atoi(argv[1]); // no. of elements
    size = atoi(argv[2]); // no. of threads


    info = (FOO *)malloc(sizeof(FOO) * size);
    // creating array of structs.
    for(int i = 0; i < size; i++) {
      info[i].N = N;
      info[i].size = size;
      info[i].tid = i;
      info[i].a = (double *)malloc(sizeof(double)*N);

      for (int j=0; j<N; j++)
        info[i].a[j] = (double)(j + 1);
    }

    // create threads
    for ( i = 0; i < size; i++) 
      pthread_create(&info[i].ptid, NULL, compute, (void *)&info[i]);
    
    // wait for them to complete
    for ( i = 0; i < size; i++)
      pthread_join(info[i].ptid, NULL);
    
    printf("The total is %g, it should be equal to %g\n", 
           sum, ((double)N*(N+1))/2);
    
    return 0;
}


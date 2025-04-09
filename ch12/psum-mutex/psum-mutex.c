#include "csapp.h"
#include "dbg.h"
#define MAXTHREADS 32

void *sum_mutex(void *vargp);
void *sum_array(void *vargp);

long gsum = 0;
sem_t mutex; /* Mutex to protect global sum */
long nelems_per_thread;
long psum[MAXTHREADS];

int main(int argc, char *argv[]) {
  long i, nelems, log_nelems, nthreads, myid[MAXTHREADS];
  pthread_t tid[MAXTHREADS];
  
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <nthreads> <long_elems>\n", argv[0]);
    exit(0);
  }
  nthreads = atoi(argv[1]);
  log_nelems = atoi(argv[2]);
  nelems = (1L << log_nelems);
  nelems_per_thread = nelems / nthreads;
  debug("main: sum from 0 to %ld with %ld threads.", nelems, nthreads);
  
  Sem_init(&mutex, 0, 1);
  
  for (i = 0; i < nthreads; ++i) {
    myid[i] = i;
    Pthread_create(&tid[i], NULL, sum_array, &myid[i]);
  }

  for (i = 0; i < nthreads; ++i) {
    Pthread_join(tid[i], NULL);
  }

  for (i = 0; i < nthreads; ++i) {
    gsum += psum[i];
  }
  
  if (gsum != nelems * (nelems - 1) / 2) {
    fprintf(stderr, "Error: result=%ld\n", gsum);
  }
  return 0;
}

void *sum_mutex(void *vargp) {
  long myid = *((long *)vargp);
  long start = myid * nelems_per_thread;
  long end = start + nelems_per_thread;
  long i;
  debug("sum_mutex: tid[%ld] start=%ld enld=%ld", myid, start, end);
  
  for (i = start; i < end; ++i) {
    P(&mutex);
    gsum += i;
    V(&mutex);
  }
  return NULL;
}

void *sum_array(void *vargp) {
  long myid = *((long *)vargp);
  long start = myid * nelems_per_thread;
  long end = start + nelems_per_thread;
  long i;
  
  int sum = 0;
  for (i = start; i < end; ++i) {
    sum += i;
  }
  psum[myid] = sum;
  return NULL;
}

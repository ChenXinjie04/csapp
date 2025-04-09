#include <semaphore.h>
#include "csapp.h"
#define N 10

sem_t mutex;
sem_t readcnt;

static void init_sem(void) {
  Sem_init(&mutex, 0, 1);
  Sem_init(&readcnt, 0, N);
}

void reader() {
  P(&readcnt);
  P(&mutex);
  V(&mutex);
  V(&readcnt);
}

void writer() {
  P(&mutex);
  V(&mutex);
}
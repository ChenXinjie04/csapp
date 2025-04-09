#include "csapp.h"
#include <pthread.h>

#define M 256
#define N 512
#define NTREHAD 4
#define M_PER_THREAD (M/NTREHAD)
#define N_PER_THREAD (N/NTREHAD)

void *thread(void *vargp);

long A[M][N] = {{}};
long B[N][M] = {{}};
long C[M][N] = {{}};
static pthread_t tid[NTREHAD];
static sem_t mutex;

int main() {
  long i;
  Sem_init(&mutex, 0, 1);
  for (i = 0; i < NTREHAD; ++i) {
    Pthread_create(&tid[i], NULL, thread, (void *)i);
  }
  for (i = 0; i < NTREHAD; ++i) {
    Pthread_join(tid[i], NULL);
  }
  return 0;
}

void *thread(void *vargp) {
  long myid = (long)vargp;
  int Mstart = myid * M_PER_THREAD;
  int Mend = Mstart + M_PER_THREAD;
  int Nstart = myid * N_PER_THREAD;
  int Nend = Nstart + N_PER_THREAD;
  long sum;
  for (int i = Mstart; i < Mend; ++i) {
    for (int j = Nstart; j < Nend; ++j) {
      sum = 0;
      for (int k = Nstart; k < Nend; ++k) {
        sum += A[i][k] * B[k][j];
      }
      C[i][j] = sum;
    }
  }
  return NULL;
}

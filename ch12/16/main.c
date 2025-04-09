#include "csapp.h"
#define MAXTHREAD 32

void *thread(void *);

int main(int argc, char *argv[]) {
  long nthread, i;
  pthread_t tid[MAXTHREAD];
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <nthread>\n", argv[0]);
    exit(0);
  }
  nthread = atoi(argv[1]);
  
  for (i = 0; i < nthread; ++i) {
    Pthread_create(&tid[i], NULL, thread, (void *)i);
  }
  
  for (i = 0; i < nthread; ++i) {
    Pthread_join(tid[i], NULL);
  }
  
  return 0;
}

void *thread(void *vargp) {
  long myid = (long)vargp;
  printf("Hello from thread [%ld]\n", myid);
  return NULL;
}

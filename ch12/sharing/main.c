#include "csapp.h"
#include <stdio.h>
#include <stdlib.h>

#define N 2

void *thread(void *);

char **ptr;

int main(int argc, char *argv[]) {
  int i;
  pthread_t tid;
  char *msgs[N] = {
    "Hello from foo",
    "Hello from bar"
  };
  
  ptr = msgs;
  for (i = 0; i < N; ++i) {
    Pthread_create(&tid, NULL, thread, (void *)i);
  }
  Pthread_exit(NULL);
}

void *thread(void *vargp) {
  int myid = (int)vargp;
  static int cnt = 0;
  printf("[%d]: %s (cnt=%d)\n", myid, ptr[myid], ++cnt);
  return NULL;
} 

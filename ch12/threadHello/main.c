#include "csapp.h"
#include <stdio.h>

void *thread(void *vargp);

int main(int argc, char *argv[]) {
  pthread_t tid1, tid2;
  pthread_create(&tid1, NULL, thread, NULL);
  pthread_create(&tid2, NULL, thread, NULL);
  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
  return 0;
}

void *thread(void *vargp) {
  printf("Hello world\n");
  return NULL;
}

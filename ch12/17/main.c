#include "csapp.h"

void *thread(void *vargp);

int main(int argc, char *argv[]) {
  pthread_t tid;
  
  Pthread_create(&tid, NULL, thread, NULL);
  Pthread_join(tid, NULL);
  return 0;
}

void *thread(void *vargp) {
  Pthread_detach(Pthread_self());
  Sleep(1);
  printf("Hello, world!\n");
  return NULL;
}

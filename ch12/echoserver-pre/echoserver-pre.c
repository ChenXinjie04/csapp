#include "csapp.h"
#include "sbuf.h"
#include <pthread.h>
#include <sys/socket.h>
#define NTHREADS 4
#define SBUFSIZE 16

void echo_cnt(int connfd);
void *thread(void *vargp);

sbuf_t sbuf;

int main(int argc, char *argv[]) {
  int listenfd, i, connfd;
  socklen_t clientlen;
  struct sockaddr_storage sockaddr;
  pthread_t tid;
  
  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(0);
  }
  listenfd = Open_listenfd(argv[1]);
  
  sbuf_init(&sbuf, SBUFSIZE);
  for (i = 0; i < NTHREADS; ++i) {
    Pthread_create(&tid, NULL, thread, NULL);
  }
  
  while (1) {
    clientlen = sizeof(struct sockaddr_storage);
    connfd = Accept(listenfd, (SA *)&sockaddr, &clientlen);
    sbuf_insert(&sbuf, connfd);
  }
}

void *thread(void *vargp) {
  Pthread_detach(Pthread_self());
  while(1) {
    int connfd = sbuf_remove(&sbuf);
    echo_cnt(connfd);
    Close(connfd);
  }
}

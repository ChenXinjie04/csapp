#include "csapp.h"
#include <sys/socket.h>

void echo(int connfd);
void *thread(void *vargp);

int main(int argc, char *argv[]) {
  int listenfd, *connfdp;
  socklen_t clientlen;
  struct sockaddr_storage clientaddr;
  pthread_t tid;
  
  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
  }

  listenfd = Open_listenfd(argv[1]);
  
  while (1) {
    clientlen = sizeof(struct sockaddr_storage);
    connfdp = (int *)malloc(sizeof(int));
    *connfdp = Accept(listenfd, (SA *)&clientaddr, &clientlen);
    Pthread_create(&tid, NULL, thread, connfdp);
  }
  return 0;
}

void *thread(void *vargp) {
  int connfd = *(int *)vargp;
  Pthread_detach(Pthread_self());
  free(vargp);
  echo(connfd);
  Close(connfd);
  return NULL;
}

void echo(int connfd) {
  int n;
  char buf[MAXLINE];
  rio_t rio;
  
  Rio_readinitb(&rio, connfd);
  while ((n = Rio_readlineb(&rio, buf, MAXLINE)) > 0) {
    Rio_writen(connfd, buf, n);
  }
  return;
}

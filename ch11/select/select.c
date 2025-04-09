#include <sys/socket.h>
#include <unistd.h>
#include "csapp.h"
#include "dbg.h"

void echo(int connfd);
void command(void);
void check_client(fd_set *ready_set);

static int maxi = 0;
static int maxfd = 0;
static fd_set read_set, ready_set;

int main(int argc, char *argv[]) {
  int listenfd, connfd;
  socklen_t clientlen;
  struct sockaddr_storage clientaddr;
  
  if(argc != 2) {
    fprintf(stderr, "usage: %s<port>\n", argv[0]);
    exit(0);
  }  
  listenfd = Open_listenfd(argv[1]);
  maxfd = listenfd;
  debug("main: listen on fd %d\n", listenfd);
  
  FD_ZERO(&read_set);
  FD_SET(listenfd, &read_set);
  FD_SET(STDIN_FILENO, &read_set);
  
  while (1) {
    ready_set = read_set;
    Select(maxfd + 1, &ready_set, NULL, NULL, NULL);
    if (FD_ISSET(STDIN_FILENO, &ready_set)) {
      command();
      FD_CLR(STDIN_FILENO, &ready_set);
    }
    if (FD_ISSET(listenfd, &ready_set)) {
      clientlen = sizeof(struct sockaddr_storage);
      connfd = Accept(listenfd, (SA*)&clientaddr, &clientlen);
      if (connfd > maxi) maxi = connfd;
      if (connfd > maxfd) maxfd = connfd;
      FD_SET(connfd, &read_set);
      FD_CLR(listenfd, &ready_set);
    }
    
    check_client(&ready_set);
  }
  return 0;
}

void check_client(fd_set *ready_setp) {
  debug("check_client: entering");
  int i = 0;
  for (i = 0; i <= maxi; ++i) {
    if (FD_ISSET(i, &ready_set)) {
      debug("check_client: fd %d is set.", i);
      echo(i);
    }
  }
  debug("check_client: exiting");
}

void command(void) {
  char buf[MAXLINE];
  
  if (!Fgets(buf, MAXLINE, stdin)) {
    exit(0);
  }
  printf("%s", buf);
}

void echo(int connfd) {
  int n;
  rio_t rio;
  char buf[MAXLINE];
  
  Rio_readinitb(&rio, connfd);
  n = Rio_readlineb(&rio, buf, MAXLINE);
  Rio_writen(connfd, buf, n);
}

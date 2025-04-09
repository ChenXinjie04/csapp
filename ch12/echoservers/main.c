#include "csapp.h"
#include "dbg.h"
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/select.h>

typedef struct {
  int maxfd;
  fd_set read_set;
  fd_set ready_set;
  int nready;
  int maxi;
  int clientfd[FD_SETSIZE];
  rio_t clientrio[FD_SETSIZE];
} pool;

void check_clients(pool *);
void init_pool(int, pool *);
void add_client(int, pool *);
void sigalrmHandler(int sig);

static int byte_cnt = 0;
static jmp_buf env;

int main(int argc, char *argv[]) {
  int listenfd, connfd;
  struct sockaddr sockaddr;
  socklen_t clientlen;
  static pool pool;

  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(0);
  }
  listenfd = open_listenfd(argv[1]);
  debug("main: listenfd=%d", listenfd);
  init_pool(listenfd, &pool);
  clientlen = sizeof(struct sockaddr);
  Signal(SIGALRM, sigalrmHandler);

  
  while (1) {
    pool.ready_set = pool.read_set;
    pool.nready = Select(pool.maxfd + 1, &pool.ready_set, NULL, NULL, NULL);
    
    if (FD_ISSET(listenfd, &pool.ready_set)) {
      connfd = Accept(listenfd, &sockaddr, &clientlen);
      log_info("main: connection on fd %d", connfd);
      add_client(connfd, &pool);
    }
    
    check_clients(&pool);
  }
  
  return 0;
}

void init_pool(int listenfd, pool *p) {
  debug("init_pool: entering");
  int i;

  p->maxi = -1;
  for (i = 0; i < FD_SETSIZE; ++i) {
    p->clientfd[i] = -1;
  }

  p->maxfd = listenfd;
  FD_ZERO(&p->read_set);
  FD_SET(listenfd, &p->read_set);
  debug("init_pool: exiting");
  return;
}

void add_client(int connfd, pool *p) {
  debug("add_client: entering");
  int i;

  p->nready -= 1;
  for (i = 0; i < FD_SETSIZE; ++i) {
    if (p->clientfd[i] == -1) {
      p->clientfd[i] = connfd;
      Rio_readinitb(&p->clientrio[i], connfd);

      FD_SET(connfd, &p->read_set);

      p->maxi = (connfd > p->maxi) ? connfd : p->maxi;
      p->maxfd = (connfd > p->maxfd) ? connfd : p->maxfd;
      break;
    }
  }
  if (i == FD_SETSIZE) app_error("add_client: Too many clients.");
  debug("add_client: exiting");
}

void check_clients(pool *p) {
  debug("check_clients: entering");
  int i;

  for(i = 0; (i <= p->maxi) && p->nready; i += 1) {
    int n, connfd = p->clientfd[i];
    rio_t rio = p->clientrio[i];
    char buf[MAXLINE];
    
    if (setjmp(env) == 1) {
      debug("time out");
      continue;
    }
    if ((connfd > 0) && FD_ISSET(connfd, &p->ready_set)) {
      p->nready -= 1;
      alarm(1);
      if ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
        byte_cnt += n;
        log_info("Server received %d (%d total) bytes on fd %d", 
                n, byte_cnt, connfd);
        Rio_writen(connfd, buf, n);
      } else {
        Close(connfd);
        p->clientfd[i] = -1;
        FD_CLR(connfd, &p->ready_set);
      }
      alarm(0);
    }
  }
  debug("check_clients: exiting");
}

void sigalrmHandler(int sig) {
  longjmp(env, 1);
}

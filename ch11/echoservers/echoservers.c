#include "csapp.h"

typedef struct {
  int maxfd;        /* Largest descriptor in read_set */
  fd_set read_set;  /* Set of all active descriptors */
  fd_set ready_set; /* Subset of descriptors ready for reading */
  int nready;       /* Number of descriptors ready for reading from select */
  int maxi;         /* High water index into client array */
  int clientfd[FD_SETSIZE];     /* Set of active descriptors */
  rio_t clientrio[FD_SETSIZE];  /* Set of active read buffers */
} pool;

int byte_cnt = 0;

void init_pool(int listenfd, pool *p);
void add_client(int connfd, pool *p);
void check_clients(pool *p);

int main(int argc, char *argv[]) {
  int listenfd;
  static pool pool;
  
  if (argc != 2) {
    sprintf(stderr, "usage: %s<port>\n", argv[0]);
    exit(0);
  }
  listenfd = Open_listenfd(argv[1]);
  init_pool(listenfd, &pool);
  
  

  return 0;
}

void init_pool(int listenfd, pool *p) {
  /* Initially, there are no connected descriptors */
  int i;
  p->maxi = -1;
  for (i = 0; i < FD_SETSIZE; ++i) {
    p->clientfd[i] = -1;
  }
  
  /* Initially, listenfd is only member of select read set */
  p->maxfd = listenfd;
  FD_ZERO(&p->read_set);
  FD_SET(listenfd, &p->read_set);
}

#ifndef __threadpool_h__
#define __threadpool_h__

#include "csapp.h"
#include "sbuf.h"

#define MAXNTHREAD 16
#define MINNTHREAD 1
#define THP_RUNNING 0
#define THP_WAITING 1
#define THP_IDLE    2

typedef void *(*thjob)(void *);

typedef struct {
  /* &begin shared variable */
  sem_t pool_mutex;
  int status[MAXNTHREAD];
  sem_t work[MAXNTHREAD];
  sem_t waiting;
  int connfd[MAXNTHREAD];
  /* &end shared variable */
  int id[MAXNTHREAD];
  int nthread;
  pthread_t tid[MAXNTHREAD];
} thpool_t;

void thpool_init(thpool_t *tp, thjob tj, void *vargp);
void thpool_double(thpool_t *tp);
void thpool_halve(thpool_t *tp);
void thpool_changeStatusTid(thpool_t *tp, pthread_t tid, int status);
void thpool_assign_work(thpool_t *tp, int connfd);
int thpool_thread_get_work(thpool_t *tp, pthread_t tid);
#endif

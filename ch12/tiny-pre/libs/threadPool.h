#ifndef __threadpool_h__
#define __threadpool_h__

#include "csapp.h"

#define MAXNTHREAD 32
#define THP_RUNNING 0
#define THP_WAITING 1
#define THP_IDLE    2

typedef void *(*thjob)(void *);

typedef struct {
  int id[MAXNTHREAD];
  int nthread;
  /* &begin shared variable */
  sem_t status_mutex;
  sem_t read_mutex;
  int status[MAXNTHREAD];
  int nrunning;
  int nwaiting;
  /* &end shared variable */
  pthread_t tid[MAXNTHREAD];
} thpool_t;

void thpool_init(thpool_t *tp, thjob tj, void *vargp);
void thpool_addthread(thpool_t *tp, void *vargp);
void thpool_rmthread(thpool_t *tp);
void thpool_adjust(thpool_t *tp);
void thpool_changeStatusTid(thpool_t *tp, pthread_t tid, int status);
void thpool_readlock(thpool_t *tp);
void thpool_readunlock(thpool_t *tp);
#endif
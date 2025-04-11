#ifndef __threadpool_h__
#define __threadpool_h__

#include "csapp.h"

#define MAXNTHREAD 32
#define MINNTHREAD 1
#define THP_RUNNING 0
#define THP_WAITING 1
#define THP_IDLE    2

typedef void *(*thjob)(void *);

typedef struct {
  int id[MAXNTHREAD];
  int nthread;
  pthread_t status_mutex_owner;
  /* &begin shared variable */
  sem_t status_mutex;
  sem_t p_mutex;
  int status[MAXNTHREAD];
  int nrunning;
  int nwaiting;
  /* &end shared variable */
  pthread_t tid[MAXNTHREAD];
} thpool_t;

void thpool_init(thpool_t *tp, thjob tj, void *vargp);
void thpool_addthread(thpool_t *tp, void *vargp);
void thpool_rmthread(thpool_t *tp);
void thpool_double(thpool_t *tp);
void thpool_halve(thpool_t *tp);
void thpool_changeStatusTid(thpool_t *tp, pthread_t tid, int status);
void thpool_lock(thpool_t *tp);
void thpool_unlock(thpool_t *tp);
void thpool_statuslock(thpool_t *tp);
void thpool_statusunlock(thpool_t *tp);
#endif
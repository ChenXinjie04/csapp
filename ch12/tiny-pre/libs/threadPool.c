#include "threadPool.h"
#include "csapp.h"
#include "dbg.h"

static thjob tj;
static void *vargp;

static void thpool_addthread(thpool_t *tp, void *vargp);
static void thpool_rmthread(thpool_t *tp);

void thpool_init(thpool_t *tp, thjob tj_, void *vargp_)
{
  int i;
  debug("thpool_init: entering tp [%p]", tp);
  Sem_init(&tp->pool_mutex, 0, 1);
  Sem_init(&tp->waiting, 0, 0);
  for (i = 0; i < MAXNTHREAD; ++i) {
    Sem_init(&tp->work[i], 0, 0);
  }
  P(&tp->pool_mutex);
  debug("thpool_init: getting lock");
  tj = tj_;
  vargp = vargp_;

  for (i = 0; i < MAXNTHREAD; ++i) {
    tp->id[i] = -1;
    tp->status[i] = THP_IDLE; /* No thread created, safe */
  }
  V(&tp->pool_mutex);
  thpool_addthread(tp, vargp);
  debug("thpool_init: exiting");
}

static void thpool_addthread(thpool_t *tp, void *vargp) {
  int i;

  /* Find a empty slot and create a new thread */
  P(&tp->pool_mutex);
  debug("get pool_mutex");
  for (i = 0; i < MAXNTHREAD; ++i) {
    if (tp->id[i] != -1) continue;
    tp->status[i] = THP_WAITING; /* change before thread creation, safe */
    Pthread_create(&tp->tid[i], NULL, tj, vargp);
    tp->id[i] = tp->nthread++;
    V(&tp->waiting);
    log_info("thpool_addthread: add thread tid [%lu] id [%d] (%d total).", tp->tid[i], tp->id[i], tp->nthread);
    break;
  }
  debug("release pool_mutex");
  V(&tp->pool_mutex);
}

/*
 * thpool_rmthread - remove a waiting thread from thread pool. Should be called 
 * after status info **locked**.
 */
static void thpool_rmthread(thpool_t *tp) {
  int i;

  P(&tp->waiting);
  P(&tp->pool_mutex);
  debug("get pool_mutex");
  for (i = 0; i < MAXNTHREAD; ++i) {
    if (tp->status[i] != THP_WAITING) continue;
    Pthread_cancel(tp->tid[i]);
    tp->id[i] = -1;
    tp->status[i] = THP_IDLE;
    tp->nthread -= 1;
    log_info("thpool_rmthread: remove thread tid [%lu] (%d total)", tp->tid[i], tp->nthread);
    break;
  }
  debug("release pool_mutex");
  V(&tp->pool_mutex);
}


void thpool_halve(thpool_t *tp) {
  debug("thpool_halve: entering");
  debug("thpool_halve: current exist thread (%d total)", tp->nthread);
  if (tp->nthread <= 1) {
    debug("thpool_halve: exiting touch lower bound");
    return ;
  }
  int halve_cnt = tp->nthread / 2;
  debug("thpool_halve: (%d) thread to be cancel.", halve_cnt);
  while (halve_cnt) {
    thpool_rmthread(tp);
    halve_cnt -= 1;
  }
  log_info("thpool_halve: halve working thread (%d total).", tp->nthread);
  debug("thpool_halve: exiting");
}

void thpool_double(thpool_t *tp) {
  debug("thpool_double: entering");
  if (tp->nthread == MAXNTHREAD) {
    debug("thpool_double: exiting touch upper bound");
    return;
  }
  int double_cnt = tp->nthread;
  while (double_cnt) {
    thpool_addthread(tp, vargp);
    double_cnt -= 1;
  }
  log_info("thpool_adjust: double working thread (%d total).", tp->nthread);
  debug("thpool_double: exiting");
}

void thpool_changeStatusTid(thpool_t *tp, pthread_t tid, int status) {
  debug("entering");
  P(&tp->pool_mutex);
  V(&tp->waiting);
  debug("get pool_mutex");
  int i;
  for (i = 0; i < MAXNTHREAD; ++i) {
    if (tp->tid[i] != tid) continue;
    if (status == THP_WAITING) {
      debug("tid [%lu] status changes into waiting", tid);
    } else if (status == THP_RUNNING) {
      debug("tid [%lu] status changes into running", tid);
    }
    tp->status[i] = status;
  }
  debug("release pool_mutex");
  V(&tp->pool_mutex);
  debug("exiting");
}

void thpool_assign_work(thpool_t *tp, int connfd) {
  int i;
  debug("wait waiting thread");
  debug("get a waiting thread");
  P(&tp->waiting);
  P(&tp->pool_mutex);
  debug("get pool_mutex");
  for (i = 0; i < MAXNTHREAD; ++i) {
    if (tp->status[i] != THP_WAITING) continue;
    tp->status[i] = THP_RUNNING;
    tp->connfd[i] = connfd;
    debug("assign work to thread [%lu]", tp->tid[i]);
    V(&tp->work[i]);
    break;
  } 
  debug("release pool_mutex");
  V(&tp->pool_mutex);
}

int thpool_thread_get_work(thpool_t *tp, pthread_t tid) {
  int i;
  for (i = 0; i < MAXNTHREAD; ++i) {
    if (tp->tid[i] != tid) continue;
    P(&tp->work[i]);
    debug("tid [%lu] get a work", tid);
    return tp->connfd[i];
  }
  log_err("thpool_thread_get_work: control should never fall here.");
error:
  return -1;
}

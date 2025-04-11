#include "threadPool.h"
#include "csapp.h"
#include "dbg.h"

static thjob tj;
static void *vargp;

void thpool_init(thpool_t *tp, thjob tj_, void *vargp_)
{
  debug("thpool_init: entering tp [%p]", tp);
  int i;
  tj = tj_;
  vargp = vargp_;
  Sem_init(&tp->status_mutex, 0, 1);
  Sem_init(&tp->p_mutex, 0, 1);

  for (i = 0; i < MAXNTHREAD; ++i) {
    tp->id[i] = -1;
    tp->status[i] = THP_IDLE; /* No thread created, safe */
  }
  thpool_addthread(tp, vargp);
  debug("thpool_init: exiting");
}

void thpool_addthread(thpool_t *tp, void *vargp) {
  int i;

  /* Find a empty slot and create a new thread */
  for (i = 0; i < MAXNTHREAD; ++i) {
    if (tp->id[i] != -1) continue;
    tp->status[i] = THP_WAITING; /* change before thread creation, safe */
    Pthread_create(&tp->tid[i], NULL, tj, vargp);
    tp->id[i] = tp->nthread++;
    log_info("thpool_addthread: add thread tid [%lu] id [%d] (%d total).", tp->tid[i], tp->id[i], tp->nthread);
    break;
  }
}

/*
 * thpool_rmthread - remove a waiting thread from thread pool. Should be called 
 * after status info **locked**.
 */
void thpool_rmthread(thpool_t *tp) {
  int i;

  for (i = 0; i < MAXNTHREAD; ++i) {
    if (tp->status[i] != THP_WAITING) continue;
    Pthread_cancel(tp->tid[i]);
    tp->id[i] = -1;
    tp->status[i] = THP_IDLE;
    tp->nthread -= 1;
    log_info("thpool_rmthread: remove thread tid [%lu]", tp->tid[i]);
    break;
  }
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
  log_info("thpool_adjust: halve working thread (%d total).", tp->nthread);
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
  debug("thpool_changeStatustid: entering");
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
  debug("thpool_changeStatustid: exiting");
}

void thpool_lock(thpool_t *tp) {
  P(&tp->p_mutex);
}

void thpool_unlock(thpool_t *tp) {
  V(&tp->p_mutex);
}

void thpool_statuslock(thpool_t *tp) {
  P(&tp->status_mutex);
}

void thpool_statusunlock(thpool_t *tp) {
  V(&tp->status_mutex);
}

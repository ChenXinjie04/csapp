#include "threadpool.h"
#include "dbg.h"

static thjob tj;
static void *vargp;

void thpool_init(thpool_t *tp, thjob tj_, void *vargp_)
{
  int i;
  tj = tj_;
  vargp = vargp_;
  Sem_init(&tp->status_mutex, 0, 1);
  Sem_init(&tp->read_mutex, 0, 1);

  for (i = 0; i < MAXNTHREAD; ++i) {
    tp->id[i] = -1;
    tp->status[i] = THP_IDLE; /* No thread created, safe */
  }
  thpool_addthread(tp, vargp);
}

void thpool_addthread(thpool_t *tp, void *vargp) {
  pthread_t tid;
  int i;

  /* Find a empty slot and create a new thread */
  for (i = 0; i < MAXNTHREAD; ++i) {
    if (tp->id[i] != -1) continue;
    tp->status[i] = THP_WAITING; /* change before thread creation, safe */
    Pthread_create(&tid, NULL, tj, vargp);
    tp->id[i] = tp->nthread++;
    log_info("thpool_addthread: add thread (%d total) with id [%d].", tp->nthread, tp->id[i]);
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
    log_info("thpool_rmthread: remove thread success.");
  }
}


/*
 * thpool_adjust - adjust the size of thread pool.
 */
void thpool_adjust(thpool_t *tp) {
  /* status info is shared variable should locked during updating */
  P(&tp->status_mutex);
  int should_halve = (tp->nthread > 2) && (tp->nrunning < tp->nthread / 2);
  if (should_halve) {
    int halve_cnt = tp->nthread / 2;
    while (halve_cnt) {
      thpool_rmthread(tp);
      halve_cnt -= 1;
    }
    tp->nthread /= 2;
    log_info("thpool_adjust: halve working thread (%d total).", tp->nthread);
  }
  
  int should_double = (tp->nthread < MAXNTHREAD) && (tp->nrunning == tp->nthread);
  if (should_double) {
    int double_cnt = tp->nthread;
    while (double_cnt) {
      thpool_addthread(tp, vargp);
      double_cnt -= 1;
    }
    tp->nthread *= 2;
    log_info("thpool_adjust: double working thread (%d total).", tp->nthread);
  }
  V(&tp->status_mutex);
  return;
}

void thpool_changeStatusTid(thpool_t *tp, pthread_t tid, int status) {
  int i;
  for (i = 0; i < MAXNTHREAD; ++i) {
    if (tp->tid[i] != tid) continue;
    P(&tp->status_mutex);
    tp->status[i] = status;
    V(&tp->status_mutex);
    log_info("thpool_changeStatusTid: thread status changes into (%d)", status);
  }
}

void thpool_readlock(thpool_t *tp) {
  P(&tp->read_mutex);
}

void thpool_readunlock(thpool_t *tp) {
  V(&tp->read_mutex);
}

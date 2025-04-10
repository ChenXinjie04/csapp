#include "csapp.h"
#include "sbuf.h"
#include "dbg.h"

void *producer(void *);
void *customer(void *);

static sbuf_t sbuf;

/* Create an empty, bounded, shared FIFO buffer with n slots */
void sbuf_init(sbuf_t *sp, int n) {
  debug("sbuf_init: entering");
  sp->buf = calloc(n, sizeof(int));
  sp->n = n;
  sp->front = sp->rear = 0;
  Sem_init(&sp->mutex, 0, 1);
  Sem_init(&sp->slots, 0, n);
  Sem_init(&sp->items, 0, 0);
  debug("sbuf_init: exiting");
}


int sbuf_isfull(sbuf_t *sp) {
  debug("sbuf_isfull: entering");
  int result;
  sem_getvalue(&sp->items, &result);
  debug("sbuf_isfull: exiting result = %d sp->n=%d", result, sp->n);
  return result == sp->n;
}

int sbuf_isempty(sbuf_t *sp) {
  int result;
  sem_getvalue(&sp->items, &result);
  return result == 0;
}


/* Clean up buffer sp */
void sbuf_deinit(sbuf_t *sp) {
  free(sp->buf);
}

/* Insert item onto the rear of shared buffer s p */
void sbuf_insert(sbuf_t *sp, int item) {
  P(&sp->slots);
  P(&sp->mutex);
  sp->buf[++sp->rear % sp->n] = item;
  V(&sp->mutex);
  V(&sp->items);
}

/* Remove and return the first item from buffer sp */
int sbuf_remove(sbuf_t *sp) {
  int item;

  P(&sp->items);
  P(&sp->mutex);
  item = sp->buf[++sp->front % sp->n];
  sp->buf[sp->front % sp->n] = 0;
  V(&sp->mutex);
  V(&sp->slots);
  return item;
}



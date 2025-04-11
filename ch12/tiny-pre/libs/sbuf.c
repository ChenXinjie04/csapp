#include "csapp.h"
#include "sbuf.h"
#include "dbg.h"

void *producer(void *);
void *customer(void *);

/* Create an empty, bounded, shared FIFO buffer with n slots */
void sbuf_init(sbuf_t *sp, int n) {
  debug("sbuf_init: entering");
  sp->buf = calloc(n, sizeof(int));
  sp->n = n;
  sp->front = sp->rear = 0;
  Sem_init(&sp->mutex, 0, 1);
  Sem_init(&sp->slots, 0, n);
  Sem_init(&sp->items, 0, 0);
  Sem_init(&sp->full, 0, 0);
  Sem_init(&sp->empty, 0, 0);
  debug("sbuf_init: exiting");
}


/* Clean up buffer sp */
void sbuf_deinit(sbuf_t *sp) {
  free(sp->buf);
}

/* Insert item onto the rear of shared buffer s p */
void sbuf_insert(sbuf_t *sp, int item) {
  debug("waiting empty_slots");
  P(&sp->slots);
  debug("waiting buf_mutex");
  P(&sp->mutex);
  sp->buf[++sp->rear % sp->n] = item;
  if (sp->rear % sp->n == sp->front % sp->n) V(&sp->full);
  debug("insert an item");
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
  if (sp->rear % sp-> n == sp->front % sp->n) V(&sp->empty);
  debug("remove an item");
  V(&sp->mutex);
  V(&sp->slots);
  return item;
}

void sbuf_Pfull(sbuf_t *sp) {
  P(&sp->full);
  debug("get sbuf_full_signal");
}

void sbuf_Pempty(sbuf_t *sp) {
  P(&sp->empty);
  debug("get sbuf_empty_signal");
}

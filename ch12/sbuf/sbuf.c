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

/* Clean up buffer sp */
void sbuf_deinit(sbuf_t *sp) {
  free(sp->buf);
}

/* Insert item onto the rear of shared buffer s p */
void sbuf_insert(sbuf_t *sp, int item) {
  P(&sp->slots);
  debug("producer: add item");
  sp->buf[++sp->rear % sp->n] = item;
  V(&sp->items);
}

/* Remove and return the first item from buffer sp */
int sbuf_remove(sbuf_t *sp) {
  int item;

  P(&sp->items);
  debug("customer: remove item");
  item = sp->buf[++sp->front % sp->n];
  sp->buf[sp->front % sp->n] = 0;
  V(&sp->slots);
  return item;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("usage: %s <nitems>\n", argv[0]);
    exit(0);
  }
  long nitems = atoi(argv[1]);
  sbuf_init(&sbuf, 10);

  pthread_t tid1, tid2;
  Pthread_create(&tid1, NULL, producer, (void *)nitems);
  Pthread_create(&tid2, NULL, customer, (void *)nitems);
  Pthread_join(tid1, NULL);
  Pthread_join(tid2, NULL);
  return 0;
}

void *producer(void *vargp) {
  long nitems = (long)vargp;
  int i;

  for (i = 0; i < nitems; ++i) {
    sbuf_insert(&sbuf, 1);
  }
  return NULL;
}

void *customer(void *vargp) {
  long nitems = (long)vargp;
  int i;

  for (i = 0; i < nitems; ++i) {
    sbuf_remove(&sbuf);
  }
  return NULL;
}

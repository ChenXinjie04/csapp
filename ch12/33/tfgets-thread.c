#include "csapp.h"
#include "dbg.h"
#include <signal.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>

void *counter_thread(void *vargp);
void *thread(void *vargp);

typedef struct {
  char *s;
  int size;
  FILE *stream;
} tfgets_t;

static tfgets_t tf;
static sem_t done, mutex;
static int cnt = -1;

char *tfgets(char *s, int size, FILE *stream) {
  debug("tfgets: entering");
  pthread_t tid1, tid2;
  Sem_init(&mutex, 0, 1);
  Sem_init(&done, 0, 0);
  tf.s = s;
  tf.size = size;
  tf.stream = stream;
  Pthread_create(&tid1, NULL, counter_thread, NULL);
  Pthread_create(&tid2, NULL, thread, &tf);
  P(&done);
  P(&mutex);
  if (cnt == 1) {
    Pthread_cancel(tid1);
    debug("tfgets: exiting");
    return tf.s;
  }
  V(&mutex);
  debug("tfgets: exiting");
  return NULL;
}

void *counter_thread(void *vargp) {
  sleep(2);
  P(&mutex);
  cnt = 0;
  V(&mutex);
  V(&done);
  return NULL;
}

void *thread(void *vargp) {
  tfgets_t *tfp = (tfgets_t *)vargp;
  tfp->s = fgets(tfp->s, tfp->size, tfp->stream);
  P(&mutex);
  cnt = 1;
  V(&mutex);
  V(&done);
  return NULL;
}

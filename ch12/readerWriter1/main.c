#include <semaphore.h>
#include <unistd.h>
#include "csapp.h"

int readcnt = 0;
int writecnt = 0;
sem_t mutex, w, cnt;

void reader(void) {
  while (1) {
    P(&mutex);
    readcnt += 1;
    if (readcnt == 1) {
      P(&w);
    }
    V(&mutex);
    
    P(&mutex);
    readcnt -= 1;
    if (readcnt == 0) { /* The last reader */
      if (writecnt) V(&cnt);
      V(&w);
    }
    V(&mutex);

  }
}

void writer(void) {
  while (1) {
    P(&cnt);
    writecnt += 1;
    P(&w);
    V(&w);
  }
}

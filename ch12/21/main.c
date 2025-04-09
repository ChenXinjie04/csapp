#include "csapp.h"

sem_t w, r, rmutex, wmutex;
int writecnt, readcnt;

void reader(void) {
  P(&r); /* Attempt read */
  P(&rmutex);
  readcnt += 1;
  if (readcnt == 1) {
    P(&w);
  }
  V(&rmutex);
  V(&r);

  P(&rmutex);
  readcnt -= 1;
  if (readcnt == 0) {
    V(&w);
  }
  V(&rmutex);
}

void writer(void) {
  P(&wmutex);
  writecnt += 1;
  if (writecnt == 1) {
    P(&r); /* Block read request */
  }
  V(&wmutex);
  P(&w);
  V(&w);
  P(&wmutex);
  writecnt -= 1;
  if (writecnt == 0) {
    V(&r);
  }
  V(&wmutex);
}

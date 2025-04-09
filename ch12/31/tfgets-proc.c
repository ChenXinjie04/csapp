#include "csapp.h"
#include "dbg.h"
#include <signal.h>

static void sigchldHandler(int sig);
static jmp_buf env;

char *tfgets(char *s, int size, FILE *stream) {
  debug("tfgets: entering");
  Signal(SIGCHLD, sigchldHandler);
  if (Fork() == 0) {
    sleep(2);
    exit(0);
  }
  if (setjmp(env) == 1) {
    debug("tfgets: longjmp");
    Signal(SIGCHLD, SIG_DFL);
    debug("tfgets: exiting");
    return NULL;
  }
  char *ret = fgets(s, size, stream);
  Signal(SIGCHLD, SIG_DFL);
  debug("tfgets: exiting");
  return ret;
}

static void sigchldHandler(int sig) {
  debug("sigchldHandler: entering");
  longjmp(env, 1);
}

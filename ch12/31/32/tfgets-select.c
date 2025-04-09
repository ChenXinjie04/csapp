#include "csapp.h"
#include "dbg.h"
#include <signal.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>

char *tfgets(char *s, int size, FILE *stream) {
  struct timeval timeval;
  timeval.tv_sec = 2;
  fd_set read_set;

  FD_ZERO(&read_set);
  FD_SET(STDIN_FILENO, &read_set);
  
  Select(1, &read_set, NULL, NULL, &timeval);
  
  if (FD_ISSET(STDIN_FILENO, &read_set)) {
    return fgets(s, size, stream);
  }
  return NULL;
}

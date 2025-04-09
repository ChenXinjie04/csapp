#include "dbg.h"
#include "csapp.h"
#include <unistd.h>

char *tfgets(char *s, int size, FILE *stream);

int main() {
  char buf[MAXLINE];
  if (tfgets(buf, MAXLINE, stdin) == NULL) {
    printf("BOOM!\n");
  } else {
    printf("%s\n", buf);
  }
  return 0;
}
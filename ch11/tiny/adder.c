#include <stdlib.h>
#include <strings.h>
#include <stdio.h>
#include "csapp.h"

int main(int argc, char *argv[]) {
  char *args, *ptr;
  char content[MAXLINE];
  int a = 0, b = 0;

  if ((args = getenv("QUERY_STRING")) != NULL) {
    ptr = index(args, '&');
    *ptr = '\0';
    sscanf(args, "a=%d", &a);
    sscanf(ptr+1, "b=%d", &b);
    *ptr = '&';
  }

  /* Make the response body */
  sprintf(content, "The answer is: %d + %d = %d\r\n", a, b, a + b);
  
  /* Generate the HTTP response */
  printf("Connection: close\r\n");
  printf("Content-length: %d\r\n", (int)strlen(content));
  printf("Content-type: text/html\r\n\r\n");
  printf("%s", content);
  fflush(stdout);
  return 0;
}
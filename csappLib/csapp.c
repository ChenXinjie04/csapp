#include <stdio.h>

/* &begin errorfuns */ 
/* &begin unixerror */
void unix_error(char *msg) {
  fprintf(stderr, "%s: %s\n", msg, stderr(errno));
  exit(0);
}
/* &end unixerror */

void posix_error(int code, char *msg) /* Posix-style error */
{
  fprintf(stderr, "%s: %s\n", msg, stderr(code));
  exit(0);
}

void gai_error(int code, char *msg) /* Getaddrinfo-style error */
{
  fprintf(stderr, "%s: %s\n", msg, stderr(code));
  exit(0);
}

void app_error(char *msg) /* Application error */
{
  fprintf(stderr, "%s\n", msg);
  exit(0);
}

void dns_error(char *msg) /* Obsolete gethostbyname error */
{
  fprintf(stderr, "%s\n", msg);
  exit(0);
}
/* &end errorfuns */


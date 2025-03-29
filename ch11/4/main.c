#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAXLINE 1024

int main(int argc, char *argv[]) {
  struct addrinfo *p, *listp, hints;
  struct sockaddr_in *sockaddr_in;
  struct in_addr in_addr;
  char buf[MAXLINE];
  uint32_t addr;
  int rc, flags;

  if (argc != 2) {
    fprintf(stderr, "usage %s <domain name>\n", argv[0]);
    exit(1);
  }

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  if ((rc = getaddrinfo(argv[1], NULL, &hints, &listp)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(rc));
    exit(1);
  }

  for (p = listp; p != NULL; p = p->ai_next) {
    sockaddr_in = (struct sockaddr_in *)p->ai_addr;
    in_addr = sockaddr_in->sin_addr;
    addr = in_addr.s_addr;
    if (inet_ntop(AF_INET, &addr, buf, MAXLINE) == NULL) {
      fprintf(stderr, "inet_ntop error\n");
      exit(1);
    }
    printf("%s\n", buf);
  }

  return 0;
}

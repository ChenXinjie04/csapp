#include <netdb.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define MAXLINE 8192

int main(int argc, char *argv[]) {
  struct addrinfo *listp, *p, hints;
  char buf[MAXLINE];
  int rc, flags;
  
  if (argc != 2) {
    fprintf(stdout, "usage: %s <domain name>\n", argv[0]);
    exit(0);
  }
  
  memset(&hints, 0, sizeof hints);
  if ((rc = getaddrinfo(argv[1], NULL, &hints, &listp)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(rc));
    exit(0);
  }
  
  flags = NI_NUMERICHOST;
  for (p = listp; p != NULL; p = p->ai_next) {
    struct sockaddr_in *sockaddr_in = (struct sockaddr_in *)p->ai_addr;
    inet_ntop(AF_INET, &sockaddr_in->sin_addr, buf, MAXLINE);
    printf("%s\n", buf);
  }
  
  freeaddrinfo(listp);
  return 0;
}
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int open_clientfd(char *hostname, char *port);
int open_listenfd(char *port);

int open_clientfd(char *hostname, char *port) {
  int clientfd, rc;
  struct addrinfo *p, *listp, hints;

  memset(&hints, 0, sizeof hints);
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_NUMERICHOST;
  hints.ai_flags |= AI_ADDRCONFIG;
  if ((rc = getaddrinfo(hostname, port, &hints, &listp)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(rc));
    exit(1);
  }

  for (p = listp; p; p = p->ai_next) {
    if ((clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) {
      continue;
    }

    if (connect(clientfd, p->ai_addr, p->ai_addrlen) != -1) {
      break;
    }
    close(clientfd);
  }

  if (!p) {
    return -1;
  } else {
    return clientfd;
  }
}

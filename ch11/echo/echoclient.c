#include "csapp.h"

int main(int argc, char *argv[]) {
  int clientfd;
  rio_t rio;
  char buf[MAXLINE];
  char *hostname, *port;
  
  if (argc != 3) {
    printf("usage: %s <hostname, port>\n", argv[0]);
    exit(0);
  }
  
  hostname = argv[1];
  port = argv[2];

  clientfd = open_clientfd(hostname, port);
  rio_readinitb(&rio, clientfd);
  while (Fgets(buf, MAXLINE, stdin) != NULL) {
    rio_writen(clientfd, buf, strlen(buf));
    rio_readlineb(&rio, buf, MAXLINE);
    Fputs(buf, stdout);
  }
  return 0;
}

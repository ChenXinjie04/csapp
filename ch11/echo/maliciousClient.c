#include "csapp.h"

int main(int argc, char *argv[]) {
  int clientfd;
  rio_t rio;
  char buf[MAXLINE], maliciousStr[] = {'H', 'e', 'l', 'l', '\n'};
  char *hostname, *port;
  
  if (argc != 3) {
    printf("usage: %s <hostname, port>\n", argv[0]);
    exit(0);
  }
  
  hostname = argv[1];
  port = argv[2];
  strcpy(buf, maliciousStr);

  clientfd = open_clientfd(hostname, port);
  rio_writen(clientfd, buf, 5);
  while(1)
    ;
  return 0;
}

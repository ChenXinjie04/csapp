#include <arpa/inet.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

int main(int argc, char *argv[]) {
  struct in_addr in_addr;
  uint32_t addr;
  if (argc != 2) {
    printf("usage: dd2hext <dotted decimal ipv4 address>\n");
  }

  if (!inet_pton(AF_INET, argv[1], &in_addr)) {
    printf("inet_pton\n");
    exit(1);
  }
  addr = ntohl(in_addr.s_addr);

  printf("0x%x\n", addr);
  return 0;
}

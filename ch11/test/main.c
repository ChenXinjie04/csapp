#include <arpa/inet.h>
#include <sys/types.h>
#include <stdio.h>

int main() {
  uint32_t a = 1;
  uint32_t b = htonl(a);
  printf("b=%08x\n", b);
  return 0;
}

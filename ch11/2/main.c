#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <arpa/inet.h>

#define MAX_BUF 1024

int main(int argc, char *argv[]) {
	struct in_addr in_addr;
	char buf[MAX_BUF];
	uint32_t address;

	if (argc < 2) {
		fprintf(stderr, "usage: hex2dd <address>\n");
		exit(1);
	}

	sscanf(argv[1], "%x", &address);
	in_addr.s_addr = htonl(address);
	if (!inet_ntop(AF_INET, &in_addr, buf, MAX_BUF)) {
		perror("inet_ntop\n");
		exit(1);
	}

	printf("%s\n", buf);
	return 0;
}

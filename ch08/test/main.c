#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
	if (fork() == 0) {
		printf("Hello from child\n");
	}
	if (fork() == 0) {
		printf("Hello from grand child\n");
	}
	printf("Hello\n");
	exit(0);
}

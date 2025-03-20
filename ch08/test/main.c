#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int rc;

void sigint_handler(int sig) {
	return;
}

int main() {
	if (signal(SIGINT, sigint_handler) == SIG_ERR)
		fprintf(stderr, "signal error\n");
	rc = sleep(5);
	printf("Slept for %d of 5 secs.\n", 5 - rc);
	exit(0);
}

#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void sigintHandler() {
	return;
}

int main() {
	if (signal(SIGINT, sigintHandler) == SIG_ERR)
		fprintf(stderr, "signal error\n");

	int rc = sleep(5);
	printf("Slept for %d of 5 secs.\n", 5 - rc);

	return 0;
}
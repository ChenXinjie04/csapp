#include <unistd.h>
#include <stdio.h>

unsigned int snooze(unsigned int secs) {
	int rc = sleep(secs);
	printf("Slept for %d of %d secs.\n", secs - rc, secs);
	return rc;
}

int main() {
	snooze(5);

	return 0;
}
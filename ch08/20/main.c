#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


int main(int argc, char *argv[], char *envp[]) {
	char *columns = getenv("COLUMNS");
	printf("Output is now %s columns wide\n", columns);

	if (execve("/usr/bin/ls", argv, envp) == -1) {
		fprintf(stderr, "execve error: %s\n", strerror(errno));	
		exit(1);
	}

	return 0;
}
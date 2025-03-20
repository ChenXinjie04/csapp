#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <wait.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
	int status;
	pid_t pid;

	while ((pid = waitpid(-1, &status, 0)) > 0) {
		if (WIFEXITED(status)) {
			printf("Child %d terminated normally with exit status %d\n", pid, WEXITSTATUS(status));
		} else {
			char buf[100];
			sprintf(buf, "Child %d terminated by signal %d", pid, WTERMSIG(status));
			psignal(status, buf);
		}
	}

	exit(1);
}

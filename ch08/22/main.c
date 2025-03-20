#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <wait.h>
#include <stdlib.h>

extern char **environ;

int mysystem(char *command) {
	char *argv[] = {"/bin/sh", "-c", command, NULL};
	execve("/bin/sh", argv, environ);

	int status;	
	if (waitpid(-1, &status, 0) > 0) {
		if (WIFEXITED(status)) {
			return WEXITSTATUS(status);
		}

		if (WIFSIGNALED(status)) {
			return WTERMSIG(status);
		}
	}

	exit(1);
}

int main(int argc, char *argv[], char *envp[]) {
	return 0;
}
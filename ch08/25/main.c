#include <stdio.h>
#include <unistd.h>
#include <setjmp.h>
#include <signal.h>

jmp_buf env;

void handler(int sig) {
	longjmp(env, 1);
}

char *tfgets(char *s, int size, FILE *stream) {
	alarm(3);
	if (setjmp(env) == 0) {
		signal(SIGALRM, handler);
		return fgets(s, size, stream);
	} else {
		return NULL;
	}
}

int main() {
	char buf[100];
	char *input = tfgets(buf, 20, stdin);

	if (input != NULL) {
		printf("INPUT: %s\n", input);
	} else {
		printf("time out\n");
	}

	return 0;
}
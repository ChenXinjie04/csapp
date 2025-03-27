#include <stdio.h>
#include <stdlib.h>

#define MAXARGS 128
#define MAXLINE 2048

void eval(char *);
int parseline(char *buf, char **argv);
int builtin_command(char **argv);

int main(int argc, char *argv[], char *envp[]) {
	char cmdline[MAXLINE];

	while (1) {
		printf("> ");
		fgets(cmdline, MAXLINE, stdin);
		if (feof(stdin)) {
			exit(0);
		}

		eval(cmdline);
	}
}

void eval(char *cmdline) {

}



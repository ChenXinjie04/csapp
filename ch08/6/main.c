#include <stdio.h>

int main(int argc, char *argv[], char *envp[]) {
	int i;

	printf("Command-line arguments:\n");
	for (i = 0; i < argc; ++i) {
		printf("\targv[%2d]: %s\n", i, argv[i]);
	}

	printf("Environment variables:\n");
	i = 0;
	while (*envp != NULL) {
		printf("\tenvp[%2d]: %s\n", i++, *envp);
		envp += 1;
	}

	return 0;
}

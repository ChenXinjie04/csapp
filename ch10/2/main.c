#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

int main() {
	int fd1 = open("foobar.txt", O_RDONLY, 0);
	int fd2 = open("foobar.txt", O_RDONLY, 0);
	char c;

	read(fd1, &c, 1);
	printf("c=%c\n", c);
	read(fd2, &c, 1);
	printf("c=%c\n", c);

	return 0;
}
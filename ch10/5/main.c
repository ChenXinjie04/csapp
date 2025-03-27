#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
	int fd1 = open("foo.txt", O_RDONLY, 0);
	int fd2 = open("bar.txt", O_RDONLY, 0);
	char c;

	read(fd2, &c, 1);
	printf("c=%c\n", c);
	dup2(fd2, fd1);
	read(fd1, &c, 1);
	printf("c=%c\n", c);

	return 0;
}
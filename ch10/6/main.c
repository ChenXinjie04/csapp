#include <stdio.h>
#include <fcntl.h>

int main() {
	int fd1 = open("foo.txt", O_RDONLY, 0);
	int fd2 = open("bar.txt", O_RDONLY, 0);
	close(fd2);
	fd2 = open("baz.txt", O_RDONLY, 0);
	printf("fd2 = %d\n", fd2);
	return 0;
}

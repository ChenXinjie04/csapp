#include <sys/fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

int main() {
	struct stat stat;
	size_t size;
	int fd = open("hello.txt", O_RDWR, 0);
	fstat(fd, &stat);
	size = stat.st_size;

	char *bufp;
	bufp = mmap(NULL, size, PROT_WRITE, MAP_SHARED, fd, 0);
	*bufp = 'J';
	return 0;
}

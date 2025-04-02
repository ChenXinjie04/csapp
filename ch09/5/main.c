#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
	off_t size;
	struct stat stat;

	int fd = open("foo.txt", O_RDWR, 0);
	fstat(fd, &stat);
	size = stat.st_size;
	char *bufp = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	*bufp = 'f';

	return 0;
}

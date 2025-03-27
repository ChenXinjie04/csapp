#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
	int fd = open("output.txt", O_RDWR, 0);

	dup2(fd, 1);
	printf("hello world\n");

	return 0;
}

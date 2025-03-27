#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>


int main(int argc, char *argv[]) {
	int fd = 0;
	char *type, *readok;
	struct stat stat;

	if (argc > 1)
		fd = atoi(argv[1]);

	fstat(fd, &stat);

	if (S_ISREG(stat.st_mode)) {
		type = "regular";
	} else if (S_ISDIR(stat.st_mode)) {
		type = "dir";
	} else {
		type = "other";
	}

	if ((stat.st_mode & S_IRUSR)) {
		readok = "yes";
	} else {
		readok = "no";
	}

	printf("type %s, read: %s\n", type, readok);

	return 0;
}

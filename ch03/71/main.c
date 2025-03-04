#include <stdio.h>
#define BUF_SIZE 12

void good_echo() {
	char buf[BUF_SIZE];
	while (1) {
		char *p = fgets(buf, BUF_SIZE, stdin);
		if (p == NULL) {
			break;
		}
		printf("%s", p);
	}
	return;
}

int main() {
	good_echo();
	return 0;
}

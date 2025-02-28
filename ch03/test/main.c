#include <stdio.h>

struct P1 {int i; char c; int j; char d; };
struct P2 {int i; char c; char d; long j; };

int main() {
	printf("The size of struct P1 is %lu\n", sizeof(struct P1));
	printf("The size of struct P2 is %lu\n", sizeof(struct P2));
	return 0;
}

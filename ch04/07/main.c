#include <stdio.h>

long push_test() {
	asm("movq %%rsp, %%rax \n\t"
			"pushq %%rsp \n\t"
			"popq %%rdx \n\t"
			"subq %%rdx, %%rax \n\t"
			:
			:
			: "%rax", "%rdx"
		);
}

int main() {
	long result = push_test();
	printf("The result after test is: %ld.", result);
	return 0;
}

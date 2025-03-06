#include <stdio.h>

long pop_test() {
	asm("movq %%rsp, %%rdi \n\t"
			"pushq $0xabcd \n\t"
			"popq %%rsp \n\t"
			"movq %%rsp, %%rax \n\t"
			"movq %%rdi, %%rsp \n\t"
			:
			:
			: "%rdi", "%rax"
		);
}

int main() {
	long result = pop_test();
	printf("The result after pop_test is: 0x%lx.", result);
	return 0;
}

cread_alt:
	movl $0, %eax
	testq %rdi, %rdi
	cmovne (%rdi), %rax
	ret
	
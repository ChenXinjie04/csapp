sum:
	xorq %rax, %rax
	andq %rsi, %rsi
	jmp test
loop:
	mrmovq (%rdi), %r8
	addq %r8, %rax
	iaddq $8, %rdi
	iaddq $-1, %rsi
test:
	jne loop
	ret
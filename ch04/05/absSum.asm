absSum:
	irmovq $8, %r8
	irmovq $1, %r9
	xorq %rax, %rax
	andq %rsi, %rsi
	jmp test
loop:
	mrmovq (%rdi), %r10
	andq %r10, %r10
	jge add
	xorq %r11, %r11	
	subq %r10, %r11
	rrmovq %r11, %r10
add:
	addq %r10, %rax
	addq %r8, %rdi
	subq %r9, %rsi
test:
	jne loop
	ret

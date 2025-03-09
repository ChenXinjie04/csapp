bubble:
	movq %rdi, %rcx
	subq $1, %rsi
	jmp test1
loop1:
	addq $8, %rcx
test1:
	subq $1, %rsi
	jg loop1
	rrmovq %rcx, %rsi
	jmp test2
innerloop:
	addq $8, %rax
loop2:
	subq %rsi, %rax
	jge outerloop
	mrmovq 8(%rax), %rdx
	mrmovq (%rax), %rcx
	jge innerloop
	movq %rcx, 8(%rax)
	movq %rdx, (%rax)
	jmp innerloop
outerloop:
	subq $8, %rsi
test2:
	cmp %rsi, %rdi
	jge done
	rrmovq %rdi, %rax
	jmp loop2
done:
	ret

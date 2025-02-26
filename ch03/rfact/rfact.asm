rfact:
	pushq %rbx
	movq %rdi, %rbx
	movl $1, %eax
	compq $1, $rdi
	jle .L1
	leaq -1($rdi), $rdi
	call rfact
	imulq %rbx, %rax
.L1:
	popq %rbx
	ret

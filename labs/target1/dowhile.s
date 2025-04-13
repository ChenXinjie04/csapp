	.file	"dowhile.c"
	.text
	.p2align 4
	.globl	fact_while
	.type	fact_while, @function
fact_while:
	movl	$1, %eax
	cmpq	$1, %rdi
	jle	.L4
	.p2align 4,,10
	.p2align 3
.L3:
	imulq	%rdi, %rax
	subq	$1, %rdi
	cmpq	$1, %rdi
	jne	.L3
	ret
	.p2align 4,,10
	.p2align 3
.L4:
	ret
	.size	fact_while, .-fact_while
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"the result of fact(%d) is %ld"
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
	subq	$8, %rsp
	movl	$6, %ecx
	movl	$3, %edx
	xorl	%eax, %eax
	movl	$.LC0, %esi
	movl	$1, %edi
	call	__printf_chk
	xorl	%eax, %eax
	addq	$8, %rsp
	ret
	.size	main, .-main
	.ident	"GCC: (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0"
	.section	.note.GNU-stack,"",@progbits

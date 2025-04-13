	.file	"switch.c"
	.text
	.p2align 4
	.globl	func
	.type	func, @function
func:
	subl	$100, %esi
	movslq	%edi, %rax
	cmpl	$6, %esi
	ja	.L8
	jmp	*.L4(,%rsi,8)
	.section	.rodata
	.align 8
	.align 4
.L4:
	.quad	.L7
	.quad	.L8
	.quad	.L6
	.quad	.L5
	.quad	.L3
	.quad	.L8
	.quad	.L3
	.text
	.p2align 4,,10
	.p2align 3
.L5:
	addq	$11, %rax
.L3:
	imull	%eax, %eax
	movl	%eax, (%rdx)
	ret
	.p2align 4,,10
	.p2align 3
.L6:
	leal	10(%rdi), %eax
	movl	%eax, (%rdx)
	ret
	.p2align 4,,10
	.p2align 3
.L7:
	leaq	(%rax,%rax,2), %rcx
	leal	(%rax,%rcx,4), %eax
	movl	%eax, (%rdx)
	ret
	.p2align 4,,10
	.p2align 3
.L8:
	xorl	%eax, %eax
	movl	%eax, (%rdx)
	ret
	.size	func, .-func
	.ident	"GCC: (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0"
	.section	.note.GNU-stack,"",@progbits

	.file	"rsum.c"
	.text
	.globl	rsum
	.type	rsum, @function
rsum:
	endbr64
	irmovl	$0, %eax
	testq	%rsi, %rsi
	jle	.L5
	pushq	%rbx
	mrmovq	(%rdi), %rbx
	irmovq $1, %r8
	irmovq $8, %r9
	subq	%r8, %rsi
	addq	%r9, %rdi
	call	rsum
	addq	%rbx, %rax
	popq	%rbx
	ret
.L5:
	ret
	.size	rsum, .-rsum
	.ident	"GCC: (Ubuntu 13.2.0-23ubuntu4) 13.2.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:

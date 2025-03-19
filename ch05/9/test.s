	.file	"test.c"
	.text
	.globl	merge
	.type	merge, @function
merge:
.LFB0:
	.cfi_startproc
	endbr64
	movq	%rdi, %r11
	movq	%rsi, %r10
	movl	$0, %eax
	movl	$0, %esi
	movl	$0, %edi
	jmp	.L2
.L3:
	addq	$1, %rsi
	movq	%r8, (%rdx,%rax,8)
	leaq	1(%rax), %rax
.L2:
	cmpq	%rsi, %rdi
	movq	%rsi, %r8
	cmovge	%rdi, %r8
	cmpq	%rcx, %r8
	jge	.L6
	movq	(%r11,%rdi,8), %r9
	movq	(%r10,%rsi,8), %r8
	cmpq	%r8, %r9
	jge	.L3
	addq	$1, %rdi
	movq	%r9, (%rdx,%rax,8)
	leaq	1(%rax), %rax
	jmp	.L2
.L7:
	movq	(%r11,%rdi,8), %r8
	movq	%r8, (%rdx,%rax,8)
	leaq	1(%rax), %rax
	leaq	1(%rdi), %rdi
.L6:
	cmpq	%rcx, %rdi
	jl	.L7
	jmp	.L8
.L9:
	movq	(%r10,%rsi,8), %rdi
	movq	%rdi, (%rdx,%rax,8)
	leaq	1(%rax), %rax
	leaq	1(%rsi), %rsi
.L8:
	cmpq	%rcx, %rsi
	jl	.L9
	ret
	.cfi_endproc
.LFE0:
	.size	merge, .-merge
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0"
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

	.file	"main.c"
	.text
	.globl	_Z6innervDv4_lS_
	.type	_Z6innervDv4_lS_, @function
_Z6innervDv4_lS_:
.LFB0:
	.cfi_startproc
	endbr64
	vpmuludq	%xmm0, %xmm1, %xmm5
	vpsrlq	$32, %xmm1, %xmm4
	vpsrlq	$32, %xmm0, %xmm2
	vpmuludq	%xmm0, %xmm4, %xmm4
	vpmuludq	%xmm1, %xmm2, %xmm2
	vpaddq	%xmm2, %xmm4, %xmm4
	vpsllq	$32, %xmm4, %xmm4
	vextractf128	$0x1, %ymm0, %xmm2
	vextractf128	$0x1, %ymm1, %xmm1
	vpmuludq	%xmm1, %xmm2, %xmm0
	vpsrlq	$32, %xmm2, %xmm3
	vpsrlq	$32, %xmm1, %xmm6
	vpmuludq	%xmm1, %xmm3, %xmm1
	vpmuludq	%xmm2, %xmm6, %xmm2
	vpaddq	%xmm2, %xmm1, %xmm1
	vpsllq	$32, %xmm1, %xmm1
	vpaddq	%xmm4, %xmm5, %xmm4
	vmovq	%xmm4, %rdx
	vpextrq	$1, %xmm4, %rax
	addq	%rax, %rdx
	vpaddq	%xmm1, %xmm0, %xmm0
	vmovq	%xmm0, %rax
	addq	%rax, %rdx
	vpextrq	$1, %xmm0, %rax
	addq	%rdx, %rax
	ret
	.cfi_endproc
.LFE0:
	.size	_Z6innervDv4_lS_, .-_Z6innervDv4_lS_
	.ident	"GCC: (Ubuntu 9.4.0-1ubuntu1~20.04.2) 9.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:

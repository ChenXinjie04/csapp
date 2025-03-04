; i in %rdi, j in %rsi, k in %rdx, dest in %rcx
store_ele:
	leaq	(%rsi, %rsi, 2), %rax		;compute 3j
	leaq	(%rsi, %rax, 4), %rax		; compute 13j
	movq	%rdi, %rsi							; copy i to %rsi
	salq	$6, %rsi								; compute 64i
	addq	%rsi, %rdi							; compute 65i
	addq	%rax, %rdi							; compute 65i + 13j
	addq 	%rdi, %rdx							; compute 65i + 13j + k
	movq	A(,%rdx,8), %rax				; load A[65i + 13j + k]
	movq	%rax, (%rcx)						; store A[65i + 13j + k] to dest	
	movl 	$3640, %eax							; set return value to 3640
	ret

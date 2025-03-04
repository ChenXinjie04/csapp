store_prod:
	movq %rdx, %rax
	cqto							;store the higher part of y in %rdx and lower part in %rax
	movq %rsi, %rcx
	sarq $63, %rcx		;store the higher part of x in %rcx
	imulq %rax, %rcx	;store the portion of y_l * x_h in %rcx
	imulq %rsi, %rdx	;store the portion of y_h * x_l in %rdx
	addq %rdx, %rcx		;store the sum of the two in %rcx
	mulq %rsi				;store the higher part of x_l * y_l in %rdx and lower part in %rax
	addq %rcx, %rdx		;add the higher part of x_l * y_l to (y_l * x_h + y_h * x_l)
	movq %rax, (%rdi) ;store the lower part of the product in the address pointed by %rdi
	movq %rdx, 8(%rdi) ;store the higher part of the product in the address pointed by %rdi + 8
	ret
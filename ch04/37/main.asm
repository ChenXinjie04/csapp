# Code to generate a combination of not-taken branch and ret
	irmovq Stack, %rsp
	irmovq rtnp, %rax
	pushq %rax
	xorq %rax, %rax
	jne target
	irmovq $1, %rax
	halt
target:
	ret
	irmovq $2, %rbx
	halt
rtnp:
	irmovq $3, %rdx
	halt
.pos 0x40
Stack:

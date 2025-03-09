irmovq $0x123, %rax
irmovq $0x321, %rdx
xorq %rcx, %rcx
cmovne %rax, %rdx
addq %rdx, %rdx
halt

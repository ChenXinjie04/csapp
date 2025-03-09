irmvoq $5, %rax
irmovq $0x100, %rsp
rmmovq %rdx, 0(%rsp)
popq %rsp
nop
nop
rrmovq $rsp, %rax

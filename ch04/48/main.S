.pos 0
	irmovq stack, %rsp  	# Set up stack pointer
	call main		# Execute main program
	halt		

.align 8
data:
  .quad 0x0000000000000004
  .quad 0x0000000000000003
  .quad 0x0000000000000002
data_end:
  .quad 0x0000000000000001

main:
  irmovq data,%rdi
	irmovq $4,%rsi
	call bubble
	ret

bubble:
	irmovq $1, %r9
	irmovq $8, %r8
	rrmovq %rdi, %rcx
	jmp test1
loop1:
	addq %r8, %rcx
test1:
	subq %r9, %rsi
	jg loop1
	rrmovq %rcx, %rsi
	jmp test2
innerloop:
	addq %r8, %rax
loop2:
	rrmovq %rax, %r10
	subq %rsi, %r10
	jge outerloop
	mrmovq 8(%rax), %rdx
	mrmovq (%rax), %rcx
	rrmovq %rdx, %r10
	subq %rcx, %r10
	cmovl %rdx, %r10
	cmovl %rcx, %rdx
	cmovl %r10, %rcx
	rmmovq %rdx, 8(%rax)
	rmmovq %rcx, (%rax)
	jmp innerloop
outerloop:
	subq %r8, %rsi
test2:
	rrmovq %rdi, %r10
	subq %rsi, %r10
	jge done
	rrmovq %rdi, %rax
	jmp loop2
done:
	ret

.pos 0x200
stack:

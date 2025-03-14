	.arch armv8.4-a+fp16+sb+ssbs
	.build_version macos,  15, 0
	.text
	.align	2
	.globl __Z5mergePlS_S_l
__Z5mergePlS_S_l:
LFB0:
	cmp	x3, 0
	ble	L1
	mov	x6, 1
	mov	x5, 0
	mov	x4, 0
	sub	x10, x2, #8
	b	L5
L3:
	add	x5, x5, 1
	mov	x7, x6
	str	x8, [x10, x6, lsl 3]
L4:
	add	x6, x6, 1
	cmp	x4, x5
	csel	x8, x4, x5, ge
	cmp	x3, x8
	ble	L12
L5:
	ldr	x9, [x0, x4, lsl 3]
	ldr	x8, [x1, x5, lsl 3]
	cmp	x9, x8
	bge	L3
	add	x4, x4, 1
	mov	x7, x6
	str	x9, [x10, x6, lsl 3]
	b	L4
L12:
	cmp	x4, x3
	bge	L6
	mov	x8, x4
	mov	x9, x7
	lsl	x6, x7, 3
	sub	x6, x6, x4, lsl 3
	add	x6, x2, x6
L7:
	ldr	x7, [x0, x4, lsl 3]
	str	x7, [x6, x4, lsl 3]
	add	x4, x4, 1
	cmp	x4, x3
	bne	L7
	add	x4, x4, x9
	sub	x7, x4, x8
L6:
	cmp	x5, x3
	bge	L1
	mov	x0, x5
	lsl	x7, x7, 3
	sub	x5, x7, x5, lsl 3
	add	x2, x2, x5
L8:
	ldr	x4, [x1, x0, lsl 3]
	str	x4, [x2, x0, lsl 3]
	add	x0, x0, 1
	cmp	x0, x3
	bne	L8
L1:
	ret
LFE0:
	.section __TEXT,__eh_frame,coalesced,no_toc+strip_static_syms+live_support
EH_frame1:
	.set L$set$0,LECIE1-LSCIE1
	.long L$set$0
LSCIE1:
	.long	0
	.byte	0x3
	.ascii "zR\0"
	.uleb128 0x1
	.sleb128 -8
	.uleb128 0x1e
	.uleb128 0x1
	.byte	0x10
	.byte	0xc
	.uleb128 0x1f
	.uleb128 0
	.align	3
LECIE1:
LSFDE1:
	.set L$set$1,LEFDE1-LASFDE1
	.long L$set$1
LASFDE1:
	.long	LASFDE1-EH_frame1
	.quad	LFB0-.
	.set L$set$2,LFE0-LFB0
	.quad L$set$2
	.uleb128 0
	.align	3
LEFDE1:
	.ident	"GCC: (Homebrew GCC 14.2.0) 14.2.0"
	.subsections_via_symbols

int odd_parity (unsigned long x) {
	int result = 0;
	while (x) {
		char bresult;
		unsigned char bx = x & 0xff;
		asm("testb %[bx], %[bx] \n\t"
				"setnp %[b]"
				: [b] "=r" (bresult)
				:	[bx] "r" (bx)
			);
		result ^= (int) bresult;
		x >>= 8;
	}
	return result;
}

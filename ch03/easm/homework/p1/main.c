int tmult_ok (long x, long y, long *dest) {
	unsigned char bresult;

	asm("imulq %[x], %[y] \n\t"
			"setae %[b]"
			: [y] "+r" (y), [b] "=r" (bresult)
			: [x] "r" (x)
		);
	*dest = y;
	return (int) bresult;
}

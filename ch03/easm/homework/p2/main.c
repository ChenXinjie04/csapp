/* Multiply two 64-bit numbers to get 128-bit result.*/
void umult_full (unsigned long x, unsigned long y, unsigned long *dest) {
	asm("movq %[x], %%rax \n\t"
			"mulq %[y] \n\t"
			"movq %%rax, %[lo] \n\t"
			"movq %%rdx, %[hi]"
		: [lo] "=m" (dest[0]), [hi] "=m" (dest[1])
		: [x] "r" (x), [y] "r" (y)
		: "%rax", "%rdx"
	);
}

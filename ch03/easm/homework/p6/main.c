/* Return sqrt(x) */
double sqrt (double x) {
	double result;
	asm("sqrtsd %[x], %[r]"
			: [r] "=x" (result)
			: [x] "x" (x)
		);
	return result;
}

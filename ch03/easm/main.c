/* Starter function for tmult_ok */
/* Multiply arguments and indicate whether it did not overflow */
int mult_ok_asm (long x, long y, long *dest) {
	long p = x * y;
	*dest = p;
	return p > 0;
}

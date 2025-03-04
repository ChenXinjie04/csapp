double funct3(int *ap, double b, long c, float *dp) {
	int i = *ap; float f = *dp;
	if (i >= b) {
		return c * f;
	} else {
		return 2 * f + c;
	}
}

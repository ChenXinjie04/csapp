double fcvt2(int *ip, float *fp, double *dp, long l) {
	int i = *ip; float f = *fp; double d = *dp;
	*ip = (int) d;
	*fp = (float) i;
	*dp = l;
	return (double) f;
}

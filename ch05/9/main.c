void merge(long src1[], long src2[], long dest[], long n) {
	long i1 = 0;
	long i2 = 0;
	long id = 0;
	while (i1 < n && i2 < n) {
		long v1 = src1[i1];
		long v2 = src2[i2];
		long take1 = v1 < v2;
		dest[id++] = take1 ? v1 : v2;
		i1 += take1;
		i2 += (1-take1);
	}
	while (i1 < n) {
		dest[id++] = src1[i1++];
	}
	while (i2 < n) {
		dest[id++] = src2[i2++];
	}
}

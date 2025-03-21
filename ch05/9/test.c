void merge(long src1[], long src2[], long dest[], long n) {
	long i1 = 0;
	long i2 = 0;
	long id = 0;
	while (i1 < n && i2 < n) {
		if (src1[i1] < src2[i2]) {
			dest[id++] = src1[i1++];
		} else {
			dest[id++] = src2[i2++];
		}
	}
	while (i1 < n) {
		dest[id++] = src1[i1++];
	}
	while (i2 < n) {
		dest[id++] = src2[i2++];
	}
}

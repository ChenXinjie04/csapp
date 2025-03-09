/* Bubble sort: Array Version */
void bubble_b(long *data, long count) {
	long i;
	long *p;
	for (i = count - 1; i > 0; i--) {
		long *end = data + i;
		for (p = data; p < end; p++) {
			long *q = p + 1;
			if (*p > *q) {
				long t = *p;
				*p = *q;
				*q = t;
			}
		}
	}
}

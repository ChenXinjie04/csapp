/* Bubble sort: Array Version */
void bubble_b(long *data, long count) {
	long *i, *last;
	for (last = data + count - 1; last > data; last--) {
		for (i = data; i < last; i++) {
			if (*(i+1) < *i) {
				long t = *(i+1);
				*(i+1) = *i;
				*i = t;
			}
		}
	}
}

#include <stdio.h>
#include <stdlib.h>
const int K = sizeof(unsigned long);

/* An implementation of memset with high performance */
void *memset(void *s, int c, size_t n) {
	long i;
	unsigned char *schar = s;
	unsigned char byteVal = (unsigned char)c;
	unsigned long chunkVal = 0;

	for (i = 0; i < K; i++) {
		chunkVal = (chunkVal << 8) | byteVal;
	}

	while (((size_t) schar) % K != 0 && n > 0) {
		*schar++ = byteVal;
		s++;
		n -= 1;
	}

	long *slong = (long *)schar;
	while (n > K) {
		*slong++ = chunkVal;
		n -= K;
	}

	schar = (unsigned char *)slong;
	while (n) {
		*schar++ = c;
		n -= 1;
	}

	return schar;
}

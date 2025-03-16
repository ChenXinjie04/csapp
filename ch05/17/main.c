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

int main() {
	unsigned char *src, *dst;
	src = malloc(100 * sizeof(unsigned char));

	printf("The end of src is: %p\n", src + 100);
	for (int i = 0; i < 100; i++) {
		src[i] = i;
		printf("%x ", src[i]);
		if (i % 10 == 9) {
			printf("\n");
		}
	}
	printf("\n");
	dst = memset(src, 1, 100);
	printf("After call memset the value in dst is: %p\n", dst);
	printf("The value in the chunck is as follows:\n");
	for (int i = 0; i < 100; i++) {
		printf("%x ", src[i]);
		if (i % 10 == 9) {
			printf("\n");
		}
	}
	printf("\n");

	return EXIT_SUCCESS;
}

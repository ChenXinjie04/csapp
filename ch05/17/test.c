#include <stddef.h>

void *memcopy(void *s, int c, size_t n) {
	unsigned char *schar = s;
	while (n > 0) {
		*schar++ = (unsigned char) c;
		n -= 1;
	}
	return s;
}
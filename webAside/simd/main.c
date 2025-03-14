#define VBYTES 32
#define VSIZE VBYTES / sizeof(data_t)
typedef long data_t;
typedef data_t vec_t __attribute__((vector_size(VBYTES)));

data_t innerv(vec_t av, vec_t bv) {
	long i;
	vec_t pv = av * bv;
	data_t result = 0;
	for (i = 0; i < VSIZE; i++) {
		result += pv[i];
	}
	return result;
}

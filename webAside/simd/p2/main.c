#define OP *
#define IDNENT 1
#define VBYTES 32
#define VSIZE VBYTES / sizeof(data_t)
typedef long data_t;
typedef data_t vec_t __attribute__((vector_size(VBYTES)));

typedef struct {
	long len;
	data_t *data;
} vec_rec, *vec_ptr;

data_t *get_vec_start(vec_ptr v) {
	return v->data;
}

int get_vec_length(vec_ptr v) {
	return v->len;
}

void simd_combination(vec_ptr v, data_t *dest) {
	long i;
	vec_t accum0;
	data_t *data = get_vec_start(v);
	int len = get_vec_length(v);
	data_t result = IDENT;

	/* Initialize all accum entries to IDENT */
	for (i = 0; i < VSIZE; ++i) {
		accum0[i] = IDENT;
	}

	/* Single step until have memory alignment */
	while (((long) data) % VBYTES && len) {
		result = result OP *data++;
		len--;
	}

	/* Step through data with four * VSIZE-way parallelism */
	while (len >= 4 * VSIZE) {
		vec_t chunk0, chunk1, chunk2, chunk3;
		chunk0 = *((vec_t *) data);
		chunk1 = *((vec_t *) (data + VSIZE));
		chunk2 = *((vec_t *) (data + 2 * VSIZE));
		chunk3 = *((vec_t *) (data + 3 * VSIZE));
		/* Only one OP will appear on the critical path each iteration */
		accum0 = accum0 OP ((chunk0 OP chunk1) OP (chunk2 OP chunk3));
		data += 4 * VSIZE;
		len -= 4 * VSIZE;
	}

}

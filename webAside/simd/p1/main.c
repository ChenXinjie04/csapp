#define VBYTES 32
#define VSIZE VBYTES / sizeof(data_t)
#define IDENT 1
#define OP *
typedef long data_t
typedef data_t vec_t __attribute__((vector_size(VBYTES)));

typedef struct {
	long len;
	data_t *data;
} vec_rec, *vec_ptr;

void simd_combine(vec_ptr v, data_t *dest) {
	long i;
	vec_t accum0, accum1, accum2, accum3;
	data_t *data = get_vec_start(v);
	int cnt = vec_length(v);
	data_t result = IDENT;
	
	/* Initialize all accum entries to IDENT */
	for (i = 0; i < VSIZE; ++i) {
		accum0[i] = IDENT;
		accum1[i] = IDENT;
		accum2[i] = IDENT;
		accum3[i] = IDENT;
	}

	/* Single step until have memory alignment */
	while (((size_t(data)) % VSIZE) != 0 && cnt) {
		result = result OP *data++;
		cnt -= 1;
	}

	/* Step through data with VSIZE-way parallelism */
	while (cnt >= VSIZE * 4) {
		vec_t chunck0 = *((vec_t *)data);
		vec_t chunck1 = *((vec_t) (data+VSIZE));
		vec_t chunck2 = *((vec_t *) (data+2*VSIZE));
		vec_t chunck3 = *((vec_t *) (data+3*VSIZE));
		accum0 = accum0 OP chunck0;
		accum1 = accum1 OP chunck1;
		accum2 = accum2 OP chunck2;
		accum3 = accum3 OP chunck3;
		data += 4 * VSIZE;
		cnt -= 4 * VSIZE;
	}

	/* Single-step through remaining elements */
	while (cnt) {
		result = result OP *data++;
		cnt -= 1;
	}

	/* Combine elements of accumulator vector */
	for (i = 0; i < VSIZE; ++i) {
		result = result OP accum0[i];
		result = result OP accum1[i];
		result = result OP accum2[i];
		result = result OP accum3[i];
	}

	/* Store result */
	*dest = result;
}

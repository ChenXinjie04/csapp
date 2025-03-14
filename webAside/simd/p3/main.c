typdef long data_t
#define VBYTES 32
#define VSIZE VBYTES/sizeof(data_t)
typedef data_t vec_t __attribute__ ((vector_size (VBYTES)));

typedef struct {
	long len;
	data_t *data;
} vec_rec, *vec_ptr;

data_t *get_vec_start(vec_ptr v) {
	return v->data;
}

int vec_length(vec_ptr v) {
	return v->len;
}

void inner_simd_v1(vec_ptr u, vec_ptr v, data_t *dest) {
	long i;
	vec_t accum;
	data_t *udata = get_vec_start(u);
	data_t *vdata = get_vec_start(v);

	int len = vec_length(u);
	data_t result = 0;

	/* Initialize all entries in accumu entries to IDENT */
	for (i = 0; i < VSIZE; ++i) {
		accum[i] = IDENT;
	}

	/* Single step until have memory alignment */
	while(((long) udata) % VBYTES != 0 && len) {
		result = result + *udata * *vdata;
		udata++;
		vdata++;
		len--;
	}

	/* Step through data with VSIZE-way parallelism */
	while (len >= VSIZE) {
		vec_t uchunk = *((vec_t *) udata);
		vec_t vchunk = *((vec_t *) vdata);
	
		accum = accum + (uchunk * vchunk);
		udata += VSIZE;
		vdata += VSIZE;
		len -= VSIZE;
	}

	/* Single-step through remaining elements */
	while (len) {
		result = result + *udata * *vdata;
		udata += 1;
		vdata += 1;
		len--;
	}

	/* Store result */	
	*dest = result;
}

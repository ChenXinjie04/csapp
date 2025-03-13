#define IDENT 0
#define OP +

typedef long data_t;
typedef struct {
	long len;
	data_t *data;
} vec_rec, *vec_ptr;

long vec_length(vec_ptr v) {
	return v->len;
}

data_t *get_vec_start(vec_ptr v) {
	return v->data;
}

/* 5 x 1 loop unrolling */
void combine5(vec_ptr v, data_t *dest) {
	long i;
	long length = vec_length(v);
	long limit = length - 4;
	data_t *data = get_vec_start(v);
	long acc = IDENT;
	
	/* Combine 2 elements at a time */
	for (i = 0; i < limit; ++i) {
		acc = (acc OP data[i]) OP data[i+1];
		acc = (acc OP data[i+2]) OP data[i+3];
		acc = acc OP data[i+4];
	}

	/* Finish any remaining elements */
	for (; i < length; i++) {
		acc = acc OP data[i];
	}

	*dest = acc;
}

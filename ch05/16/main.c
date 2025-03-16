typedef long data_t;

typedef struct {
	long len;
	data_t *data;
} vec_rec, *vec_ptr;

data_t *get_vec_start(vec_ptr v) {
	return v->data;
}

long vec_length(vec_ptr v) {
	return v->len;
}

/* Inner product. Accumulate in temporary */
void inner5(vec_ptr u, vec_ptr v, data_t *dest) {
	long len = vec_length(u);
	data_t *udata = get_vec_start(u);
	data_t *vdata = get_vec_start(v);
	data_t sum = (data_t) 0;

	while (len > 6) {
		sum = sum + (((udata[0] * vdata[0] + udata[1] * vdata[1]) + 
		(udata[2] * vdata[2] + udata[3] * vdata[3])) + 
		(udata[4] * vdata[4] + udata[5] * vdata[5]));
		udata += 6;
		vdata += 6;
		len -= 6;
	}
	while (len > 0) {
		sum = sum + udata[0] * vdata[0];
		udata++;
		vdata++;
		len--;
	}
	*dest = sum;
}

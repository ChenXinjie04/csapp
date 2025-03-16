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
/* Answer: The capacity of floating point multiplication unit limit the performance */
void inner6(vec_ptr u, vec_ptr v, data_t *dest) {
	long i;
	long len = vec_length(u);
	data_t *udata = get_vec_start(u);
	data_t *vdata = get_vec_start(v);
	data_t sum0, sum1, sum2, sum3, sum4, sum5;
	sum0 = sum1 = sum2 = sum3 = sum4 = sum5 = (data_t) 0;

	while (len > 36) {
		sum0 = sum0 + (((udata[0] * vdata[0] + udata[1] * vdata[1]) + 
		(udata[2] * vdata[2] + udata[3] * vdata[3])) + 
		(udata[4] * vdata[4] + udata[5] * vdata[5]));
		udata += 6; vdata += 6;

		sum1 = sum1 + (((udata[0] * vdata[0] + udata[1] * vdata[1]) +
		(udata[2] * vdata[2] + udata[3] * vdata[3])) +
		(udata[4] * vdata[4] + udata[5] * vdata[5]));
		udata += 6; vdata += 6;

		sum2 = sum2 + (((udata[0] * vdata[0] + udata[1] * vdata[1]) +
		(udata[2] * vdata[2] + udata[3] * vdata[3])) +
		(udata[4] * vdata[4] + udata[5] * vdata[5]));
		udata += 6; vdata += 6;

		sum3 = sum3 + (((udata[0] * vdata[0] + udata[1] * vdata[1]) +
		(udata[2] * vdata[2] + udata[3] * vdata[3])) +
		(udata[4] * vdata[4] + udata[5] * vdata[5]));
		udata += 6; vdata += 6;

		sum4 = sum4 + (((udata[0] * vdata[0] + udata[1] * vdata[1]) +
		(udata[2] * vdata[2] + udata[3] * vdata[3])) +
		(udata[4] * vdata[4] + udata[5] * vdata[5]));
		udata += 6; vdata += 6;

		sum5 = sum5 + (((udata[0] * vdata[0] + udata[1] * vdata[1]) +
		(udata[2] * vdata[2] + udata[3] * vdata[3])) +
		(udata[4] * vdata[4] + udata[5] * vdata[5]));
		udata += 6; vdata += 6;
		len -= 36;
	}
	while (len > 0) {
		sum0 = sum0 + udata[0] * vdata[0];
		udata++; vdata++;

		len--;
	}
	*dest = sum0 + sum1 + sum2 + sum3 + sum4 + sum5;
}

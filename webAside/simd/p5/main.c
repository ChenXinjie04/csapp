#define VBYTES 32
#define VSIZE VBYTES / sizeof(data_t)
typedef long data_t
typedef data_t vec_t __attribute__((vector_size(VBYTES)));

double poly_simd_v1(double a[], double x, long degree) {
	long i;
	vec_t accum;
	double *data = a;
	long cnt = degree + 1;
	double result = 0;
	double xpwr = 1.0;

	vec_t xvv; /* Vector of x^{VSIZE} */
	vec_t xpwrv; /* Vector of increasing powers of x */

	/* Initialize accum entries to 0, and compute x^{VSIZE} */
	xpwr = 1.0;
	for (i = 0; i < VSIZE; ++i) {
		accum[i] = 0;
		xpwr *= x;
	}

	/* Create a vector of all x^{VSIZE} */
	for (i = 0; i < VSIZE; ++i)
		xvv[i] = xpwr;

	xpwr = 1;
	/* Single step until have memory alignment */
	while (((size_t) data) % VBYTES != 0 && cnt > 0) {
		result += *data * xpwr;
		xpwr *= x;
		data++;
		cnt--;
	}

	/* Create vector with values xpwr, xpwr*v, ... */
	for (i = 0; i < VSIZE; ++i) {
		xpwrv[i] = xpwr;
		xpwr *= x;
	}

	/* Main loop. Accumulate sums in parallel */
	while (len > VSIZE) {
		vec_t chunck = *((vec_t *) data);
		accum += chunck * xpwrv;
		xpwrv *= xvv;
		data += VSIZE;
		len -= VSIZE;
	}

	/* Extract accumulated value */
	for (i = 0; i < VSIZE; ++i) {
		result += accum[i];
	}

	/* Single step until done */
	while (len > 0) {
		result += *data * xpwr;
		xpwr *= x;
		data++;
		len--;
	}

	return result;
}

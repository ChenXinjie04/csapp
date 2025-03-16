/*
* Function: psum1
* Usage: psum1(a, p, n);
* -----------------------
* This function computes the prefix sum without write/store dependencies.
*/
void psum1(float a[], float p[], long n) {
	float lastVal;
	float val;
	float a0, a1, a2, a3, tmp;
	int i = 0;
	while (n > 4) {
		a0 = a[i]; a1 = a[i] + a[i+1];
		tmp = a[i+2] + a[i+3];
		a2 = a1 + a[i+2];
		a3 = a1 + tmp;
		
		p[i] = a0 + lastVal;
		p[i+1] = a1 + lastVal;
		p[i+2] = a2 + lastVal;

		val = a3 + lastVal;
		p[i+3] = val;

		lastVal = val;
		i += 4;
		n -= 4;
	}

	while (n) {

	}

}

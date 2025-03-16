/*
* Function: psum1
* Usage: psum1(a, p, n);
* -----------------------
* This function computes the prefix sum without write/store dependencies.
*/
void psum1(float a[], float p[], long n) {
	float lastVal, val;
	val = p[0] = a[0];
	for (int i = 1; i < n; ++i) {
		val = lastVal + a[i];
		p[i] = val;
		lastVal = val;
	}
}

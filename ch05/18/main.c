double polyExpression(double a[], double x, int degrees) {
	long i;
	double result;
	double acum0 = a[degrees], acum1 = a[degrees - 1];
	double x2pow = x * x;

	i = 0;
	while (degrees >= 3) {
		acum0 = acum0 * x2pow + (a[degrees] * x2pow + a[degrees - 2]);
		acum1 = acum1 * x2pow + (a[degrees - 1] * x2pow + a[degrees - 3]);

		degrees -= 4;
	}
	acum0 = acum0 * x + acum1;

	while (degrees >= 0) {
		acum0 = acum0 * x + a[degrees];
		degrees--;
	}
	result = acum0;

	return result;
}

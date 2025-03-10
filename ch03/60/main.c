long loop(long x, int n) {
	long result = 0;
	long mask;
	for (mask = 0; mask != 0; mask = mask << n) {
		result |= x & mask;
	}
	return result;
}
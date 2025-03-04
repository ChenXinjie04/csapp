long switch_prob(long x, long n) {
	long result = x;
	switch(n) {
		/* Fill in code here */
		case 60:
		case 62:
			result = 8 * x;
			break;
		case 63:
			result = x >> 3;
			break;
		case 64:
			x = (x << 4) - x;
		case 65:
			x = x * x;
		default:
			result = 75 + x;
	}
	return result;
}

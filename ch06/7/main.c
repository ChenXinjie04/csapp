const int N = 10;
int productarray3d(int a[N][N][N]) {
	int i, j, k, product = 1;

	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			for (k = 0; k < N; ++k) {
				product *= a[i][j][k];
			}
		}
	}

	return product;
}

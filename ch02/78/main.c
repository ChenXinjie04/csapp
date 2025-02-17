int divide_power2(int x, int k) {
    int is_neg = x >> 31;
    (is_neg && (x = x + (1 << k) - 1));
    return x >> k;
}

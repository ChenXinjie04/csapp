int mul3div4(int x) {
    int mul3 = (x << 1) + x;
    int neg_flag = mul3 >> 31;
    (neg_flag && (mul3 = mul3 + (1 << 2) - 1));
    return mul3 >> 2;
}

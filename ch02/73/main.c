/* Addition that saturates to TMin or TMax */
int saturating_add(int x, int y) {
    int sum = x + y;
    int w = sizeof(int) << 3;
    int mask = 1 << (w - 1);
    int pos_over = !(x & mask) && !(y & mask) && (sum & mask);
    int neg_over = (x & mask) && (y & mask) && !(sum & mask);
    (pos_over && (sum = ~(1 << (w - 1)))) || (neg_over && (sum = (1 << (w - 1))));
    return sum;
}

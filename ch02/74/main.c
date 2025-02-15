/* Determine whether arguments can be subtracted without overflow */
int tsub_ok(int x, int y) {
    int sub = x - y;
    int pos_overflow = x > 0 && y < 0 && sub < 0;
    int neg_overflow = x < 0 && y > 0 && sub > 0;

    return !pos_overflow && !neg_overflow;
}

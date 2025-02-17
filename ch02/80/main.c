#include <limits.h>

int threefourths(int x) {
    int neg_flag = x & INT_MIN;
    int m30 = x & ~0x3;
    int l2 = x & 0x3;
    int m30m3d4 = ((m30 >> 2) << 1) + (m30 >> 2);
    int bias = (1 << 2) - 1;
    int l2m3 = (l2 << 1) + l2;
    (neg_flag && (l2m3 = l2m3 + bias));
    int l2m3d4 = l2m3 >> 2;
    return m30m3d4 + l2m3d4;
}

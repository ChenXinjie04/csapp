#include <stdio.h>

/* Test any bit in x equals 1. */
int test_a(int x) {
    /* Not Not x */
    return !!x;
}

/* Test any bit in x equals 0. */
int test_b(int x) {
    /* invert x */
    int invert_x = ~x;
    /* Not Not invert_x */
    return !!invert_x;
}

/* Test any bit in least significant byte of x equals 1. */
int test_c(int x) {
    /* make mask */
    int mask = 0xFF;
    return !!(x & mask);
}

/* Test any bit in the most significant byte of x equals 0. */
int test_d(int x) {
    /* shift x */
    int shift_x = x >> ((sizeof(int) - 1) << 3);
    /* invert x */
    int invert_x = ~shift_x;
    /* Not Not invert_x */
    return !!invert_x;
}

int main() {
    int x = 0xfe123456;
    printf("test_d: %d\n", test_d(x));
    return 0;
}

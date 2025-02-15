#include <stdio.h>

int int_size_is_32() {
    /* Set most significant bit (msb) of 32-bit machine */
    int set_msb = 1 << 31;
    int beyond_msb = set_msb << 1;

    return set_msb && !beyond_msb;
}

int int_size_is_32_16bit() {
    /* Set most significant bit (msb) of 16-bit machine */
    int set_msb = 1 << 15 << 15 << 1;
    int beyond_msb = set_msb << 1;

    return set_msb && !beyond_msb;
}

int main() {
    printf("int_size_is_32: %d\n", int_size_is_32());

    return 0;
}

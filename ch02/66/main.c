#include <stdio.h>

int leftmost_ones(unsigned x) {
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return (x >> 1) + (x && 1);
}

int main() {
    unsigned x = 0x00000001;
    unsigned result = leftmost_ones(x);
    printf("%x\n", result);

    return 0;
}
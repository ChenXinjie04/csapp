#include <stdio.h>

int fits_bits(int x, int n) {
    int w = sizeof(int) << 3;
    int offset = w - n;
    return x == (x << offset >> offset);
}

int main() {
    int result = fits_bits(0x00045678, 20);
    printf("result: %d\n", result);

    return 0;
}

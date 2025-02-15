#include <stdio.h>

unsigned rotate_left(unsigned x, int n) {
    int w = sizeof(unsigned) << 3;
    unsigned left_part = x >> (w - n);
    return (x << n) | left_part;
}

int main() {
    unsigned x = 0x12345678;
    int n = 20;
    unsigned result = rotate_left(x, n);
    printf("%x", result);
    return 0;
}

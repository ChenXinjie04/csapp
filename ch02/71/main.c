#include <stdio.h>

typedef unsigned packed_t;

int xbyte(packed_t word, int bytenum) {
    int w = sizeof(unsigned) << 3;
    int left_offset = w - ((bytenum + 1) << 3);
    int right_offset = w - 8;
    return (int) (word << left_offset) >> right_offset;
}

int main() {
    int x = 0x12ff3456;
    printf("%08x", xbyte(x, 3));

    return 0;
}

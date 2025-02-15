#include <stdio.h>

unsigned int replace_byte(unsigned x, int i, unsigned char b) {
    unsigned int leftOp = b << (i << 3);
    unsigned int mask = 0xFF << (i << 3);
    return (x & ~mask) | leftOp;
}

int main() {
    unsigned int result = replace_byte(0x12345678, 2, 0xAB);
    printf("0x%x\n", result);

    return 0;
}

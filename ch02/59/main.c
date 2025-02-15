#include <stdio.h>

unsigned int mix(unsigned int x, unsigned int y) {
    return (x & 0xFF) | (y & ~0xFF);
}

int main() {
    printf("0x%x\n", mix(0x89ABCDEF, 0x76543210));
    return 0;
}

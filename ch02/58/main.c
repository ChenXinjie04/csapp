#include <stdio.h>

typedef unsigned char *byte_pointer;

unsigned char get_byte(byte_pointer x, int n) {
    return x[n];
}

int is_little_endian() {
    int x = 0x12345678;
    return get_byte((byte_pointer) &x, 0) == 0x78;
}

int main() {
    printf("%d\n", is_little_endian());
    return 0;
}

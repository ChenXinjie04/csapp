#include <stdio.h>

typedef unsigned char *byte_pointer;

void show_bytes(byte_pointer start, size_t len) {
    size_t i;
    for (i = 0; i < len; i++) {
        printf(" %2x", start[i]);
    }
    printf("\n");
}

void show_int(int x) {
    show_bytes((byte_pointer) &x, sizeof(int));
}

void show_float(float x) {
    show_bytes((byte_pointer) &x, sizeof(float));
}

void show_pointer(void *x) {
    show_bytes((byte_pointer) &x, sizeof(void *));
}

void show_double(double x) {
    show_bytes((byte_pointer) &x, sizeof(double));
}

void show_long(long x) {
    show_bytes((byte_pointer) &x, sizeof(long));
}

void show_short(short x) {
    show_bytes((byte_pointer) &x, sizeof(short));
}

int main() {
    int x = 0x12345678;
    show_int(x);

    return 0;
}

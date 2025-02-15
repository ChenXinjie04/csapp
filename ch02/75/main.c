#include <stdio.h>
#include <stdint.h>

int signed_high_prod(int x, int y) {
    int64_t mul = (int64_t)x * y;
    return mul >> 32;
}

unsigned unsigned_high_prod(unsigned x, unsigned y) {
    int sign_x = x >> 31;
    int sign_y = y >> 31;
    return signed_high_prod(x, y) + x * sign_y + y * sign_x;
}

unsigned unsigned_high_prod_for_test(unsigned x, unsigned y) {
    uint64_t mul = (uint64_t)x * y;
    return mul >> 32;
}

int main() {
    int x = 0x12345678;
    int y = 0x87654321;
    printf("is equal: %d\n", unsigned_high_prod_for_test(x, y) == unsigned_high_prod(x, y));

    return 0;
}

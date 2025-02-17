#include <assert.h>
#include <stdio.h>

int A(int k) {
    return -1 << k;
}

int B(int k, int j) {
    return (1 << (k + j)) - (1 << (j));
}

int main() {
    assert(A(3) == 0xFFFFFFF8);
    assert(B(3, 4) == 0x00000070);
    return 0;
}

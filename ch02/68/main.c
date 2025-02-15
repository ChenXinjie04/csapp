#include <stdio.h>

int lower_one_mask(int n) {
    /* Left shift n */
    unsigned mask_n = 1 << (n - 1);
    /* left shift one more */
    unsigned mask_n_1 = mask_n << 1;
    /* mask minus 1 */
    return mask_n_1 - 1;
}

int main() {
    printf("%x", lower_one_mask(17));

    return 0;
}

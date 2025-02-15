#include <stdio.h>

/* Return 1 when any odd bit of x equals 1; 0 otherwise.
    Assume w=32 */
int any_odd_one(unsigned x ) {
    /* Make a odd bit mask */
    unsigned mask = 0xAAAAAAAA;
    /* Check if any odd bit is 1 */
    int masked_x = x & mask;
    /* Not Not masked_x */
    return !!masked_x;
}

int main() {
    unsigned x = 0x5555555a;
    printf("%d\n", any_odd_one(x));

    return 0;
}

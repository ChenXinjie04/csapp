#include <stdio.h>

int int_shifts_are_arithmetic() {
    /* init a int variable */
    int x = 0x80000000;
    int shift_amount = ((sizeof(int)) << 3) - 1;
    /* shift right x */
    int shift_x = x >> shift_amount;
    /* test x equals -1 */
    return shift_x == -1;
}

int main() {
    printf("int_shifts_are_arithmetic: %d\n", int_shifts_are_arithmetic());
    return 0;
}

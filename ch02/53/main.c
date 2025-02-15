#include <stdio.h>

#define POS_INFINITY 1e400
#define NEG_INFINITY -(POS_INFINITY)
#define NEG_ZERO -(1.0/POS_INFINITY)

int main() {
    printf("POS_INFINITY: %f\n", POS_INFINITY);
    printf("NEG_INFINITY: %f\n", NEG_INFINITY);
    printf("NEG_ZERO: %f\n", NEG_ZERO);

    return 0;
}

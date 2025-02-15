#include <stdint.h>

/* Determine whether arguments can be multiplied without overflow */
int tmult_ok(int x, int y) {
    int64_t p = (int64_t) x * y;
    return p == (int) p;
}

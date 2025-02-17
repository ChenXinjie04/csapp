#include <limits.h>

typedef unsigned float_bits;

int bits_length(unsigned x) {
    int length = 0;
    while (x) {
        x >>= 1;
        length++;
    }
    return length;
}

unsigned gen_mask(int length) {
    return (1 << length) - 1;
}

/* Compute (float) i */
float_bits float_i2f(int i) {
    unsigned sign = i & 0x80000000;
    unsigned f = i & 0x7FFFFFFF;
    unsigned exp = 0;
    unsigned bias = 127;

    int length = bits_length(f);
    exp = length + bias - 1;

    unsigned exp_frac = 0;
    unsigned frac = 0;
    if (length < 23) {
        frac = f;
        exp_frac = frac << (23 - length);
    } else {
        int offset = length - 23;
        frac = f >> offset; 
        int round_mid = 1 << (offset - 1);
        int round_part = f & gen_mask(offset);
        if (round_part > round_mid || (round_part == round_mid && (frac & 1))) {
            frac++;
            if (frac == 0x800000) {
                exp++;
                frac = 0;
            }
        }
    }

    return sign | (exp << 23) | frac;
}

typedef unsigned float_bits;

/* Comput 2*f. If f is NaN, then return f. */
float_bits float_twice(float_bits f) {
    unsigned sign = f >> 31;
    unsigned exp = f >> 23 & 0xFF;
    unsigned frac = f & 0x7FFFFF;

    int is_nan_or_inf = (exp == 0xFF);
    if (is_nan_or_inf) {
        return f;
    }

    /* Denormalize */
    if (exp == 0) {
        unsigned overflow = frac >> 22;
        if (overflow) {
            exp = 1;
            frac = (frac << 1);
        } else {
            frac = frac << 1;
        }
    } else if (exp == 0xff - 1) {
        exp = 0xFF;
        frac = 0;
    } else {
        exp = exp + 1;
    }

    return (sign << 31) | (exp << 23) | frac;
}
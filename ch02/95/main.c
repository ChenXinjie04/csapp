typedef unsigned float_bits; 

float_bits float_half(float_bits f) {
    unsigned sign = f >> 31;
    unsigned exp = f >> 23 & 0xFF;
    unsigned frac = f & 0x7FFFFF;

    int is_nan_or_inf = (exp == 0xFF);
    if (is_nan_or_inf) {
        return f;
    }

    if (exp == 0) {
        unsigned round = (frac & 0x3) == 0x3;
        frac = (frac >> 1) + round;
        frac = frac & 0x7FFFFF;
    } else if (exp == 1) {
        unsigned round = (frac & 0x3) == 0x3;
        frac = (1 << 22) + (frac >> 1) + round;
        unsigned overflow = frac >> 23;
        if (overflow) {
            exp = 1;
        } else {
            exp = 0;
        }
        frac = frac & 0x7FFFFF;
    } else {
        exp = exp - 1;
    }

    return (sign << 31) | (exp << 23) | frac;
}

typedef unsigned float_bits;

float_bits float_f2i(float_bits f) {
    unsigned sign = f >> 31;
    unsigned exp = f >> 23 & 0xFF;
    unsigned frac = f & 0x7FFFFF;

    int is_nan_or_inf = (exp == 0xFF);
    if (is_nan_or_inf) {
        return 0x80000000;
    }

    int bias = 127;
    int E = exp - bias;
    if (E > 31) {
        return 0x80000000;
    } else if (E < 0) {
        return 0;
    }

    unsigned M = frac | 0x800000;
    unsigned shift = 23 - E;
    if (shift > 0) {
        M = M >> shift;
    } else {
        M = M << -shift;
    }

    if (sign) {
        M = -M;
    }

    return M;
}

int main() {
    float x = -3;

    float_bits f = *(float_bits*)&x;
    float_bits i = float_f2i(f);
    printf("%d", i);

    return 0;
}

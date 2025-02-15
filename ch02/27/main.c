int uadd_ok(unsigned x, unsigned y) {
    return x + y >= x;
}

int main() {
    unsigned x = 0x80000000;
    unsigned y = 0x00000000;
    printf("uadd_ok: %d\n", uadd_ok(x, y));
    return 0;
}
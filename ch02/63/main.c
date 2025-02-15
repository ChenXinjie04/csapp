unsigned srl(unsigned x, int k) {
    /* Perform shift arithmetically */
    unsigned xsra = (int) x >> k;
    /* get width */
    int w = 8*sizeof(int);
    /* get mask */
    unsigned mask = (1 << (w - k)) - 1;
    /* and with mask */
    return xsra & mask;
}

unsigned sra(unsigned x, int k) {
    /* Perform shift logically */
    unsigned xsrl = (unsigned) x >> k;
    int w = 8*sizeof(int);
    unsigned msb = x & 0x80000000;
    unsigned mask = -(msb << (w - k));
    return xsrl | mask;
}

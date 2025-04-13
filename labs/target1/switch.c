void func(int x, int n, int *dest) {
    long val = x;
    switch(n) {
        case 100:
            val *= 13;
            break;
        case 102:
            val += 10;
            break;
        case 103:
            val += 11;
        case 104:
        case 106:
            val *= val;
            break;
        default:
            val = 0;
    }
    *dest = val;
}

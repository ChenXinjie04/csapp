#include <stdio.h>

#define KSIZE 1024

void copyFromKernel(void *userDest, int maxLen) {
    int len = KSIZE < maxLen ? KSIZE : maxLen;
    printf("len: %u\n", len);
}

int main() {
    copyFromKernel(NULL, -1024);    
    return 0;
}

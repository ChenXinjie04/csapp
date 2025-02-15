#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void* calloc(size_t nmemb, size_t size);

int main(){
    return 0;
}

void *calloc(size_t nmemb, size_t size) {
    /**
     * The calloc function allocates memory for an array of nmemb elements of size bytes each.
     */
    if (nmemb == 0 || size == 0) {
        return NULL;
    }
    
    uint64_t requestSize =  nmemb * (uint64_t) size;
    uint32_t requeiredSize = (uint32_t) requestSize;
    if (requestSize != requeiredSize) {
        return NULL;
    }

    void *result = malloc(requeiredSize);

    memset(result, 0, requeiredSize);

    return result;
}

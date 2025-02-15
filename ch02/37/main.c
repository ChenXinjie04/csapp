#include <stdint.h>
#include <stdlib.h>

void* copy_elements(void *ele_src[], int ele_cnt, size_t ele_size) {
    uint64_t total_size = ele_cnt * ele_size;
    if (total_size != (int) total_size) {
        return NULL;
    }

    void *result = malloc(total_size);

    for (int i = 0; i < ele_cnt; i++) {
        memcpy(result, ele_src[i], ele_size);
        result += ele_size;
    }

    return result;
}

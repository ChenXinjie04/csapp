#include <math.h>
#include <assert.h>
typedef enum {NEG, ZERO, POS, OTHER} range_t;

range_t find_range(float x) {
    asm(
        "vxorps %%xmm1, %%xmm1, %%xmm1 \n\t"
        "vucomiss %%xmm1, %%xmm0 \n\t"
        "movq $1, %%rax \n\t"
        "movq $2, %%r8 \n\t"
        "movq $0, %%r9 \n\t"
        "movq $3, %%r10 \n\t"
        "cmova %%r8, %%rax \n\t"
        "cmovb %%r9, %%rax \n\t"
        "cmovp %%r10, %%rax \n\t"
        :
        :
        : "%xmm1", "%rax"
    );
}

void test() {
	range_t answer;
	for (double i = -10; i < 10; i += 0.1) {
		if (i < 0) {
			answer = NEG;
		} else if (i == 0) {
			answer = ZERO;
		} else {
			answer = POS;
		}
		assert(find_range(i) == answer);
	}
	double i = NAN;
	assert(find_range(i) == OTHER);
}

int main() {
    test();
    return 0;
}

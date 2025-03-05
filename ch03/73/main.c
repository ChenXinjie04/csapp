#include <assert.h>
#include <math.h>

typedef enum {NEG, ZERO, POS, OTHER} range_t;

range_t find_range(float x) {
	range_t result = 0;
	asm(
		"vxorps %%xmm1, %%xmm1, %%xmm1 \n\t"
		"vucomiss %%xmm1, %[x]\n\t"
		"jp .p \n\t"
		"ja .a \n\t"
		"jb .b \n\t"
		"je .e \n\t"
		".a: \n\t"
		"movl $2, %[r] \n\t"
		"jmp .done \n\t"
		".b: \n\t"
		"movl $0, %[r] \n\t"
		"jmp .done \n\t"
		".e: \n\t"
		"movl $1, %[r] \n\t"
		"jmp .done \n\t"
		".p: \n\t"
		"movl $3, %[r] \n\t"
		".done:"
		: [r] "=r" (result)
		: [x] "x" (x)
		: "%xmm1"
	);
	return result;
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

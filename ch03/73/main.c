#include <assert.h>

typedef enum {NEG, ZERO, POS, OTHER} range_t;

range_t find_range(float x) {
	range_t result = 0;
	asm(
		"vxorps %%xmm1, %%xmm1, %%xmm1 \n\t"
		"vucomiss %%xmm1, %[x]\n\t"
		"ja .a \n\t"
		"jb .b \n\t"
		"je .e \n\t"
		"jp .p \n\t"
		".a: \n\t"
		"movl $2, %[r] \n\t"
		".b: \n\t"
		"movl $0, %[r] \n\t"
		".e: \n\t"
		"movl $1, %[r] \n\t"
		".p: \n\t"
		"movl $3, %[r]"
		: [r] "=r" (result)
		: [x] "x" (x)
	);
	return result;
}

int main() {
	assert(0 == find_range(0.0));
	return 0;
}

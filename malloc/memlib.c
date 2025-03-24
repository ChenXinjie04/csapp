#include "memlib.h"
#include <stdlib.h>
#include <sys/errno.h>
#include <stdio.h>

#define MAX_HEAP (1 << 20)

static char *mem_heap;
static char *mem_brk;
static char *mem_max_heap;

/*
* mem_init - Initialize the memory system.
*/
void mem_init(void) {
	mem_heap = (char *)malloc(MAX_HEAP);
	mem_brk = mem_heap;
	mem_max_heap = mem_brk + 1;
}

/*
* mem_sbrk - Extends the heap by incr bytes and return the start address of the
* new area. In this model, the heap cannot be shrunk.
*/
void *mem_sbrk(int incr) {
	char *old_brk = mem_brk;

	if ((incr < 0) || (mem_brk + incr > mem_max_heap)) {
		errno = ENOMEM;
		fprintf(stderr, "No memory");
		exit(1);
	}
	mem_brk += incr;
	return old_brk;
}

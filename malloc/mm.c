#include "memlib.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

/* Basic constants and macros */
#define WSIZE 4
#define DSIZE 8
#define CHUNKSIZE (1 << 12)

#define MAX(x, y) ((x) > (y)) ? (x) : (y)

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc) ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))

/* Read the size and alloc fields at address p */
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

/* Given block pointer bp, compute the address of its header and footer */
#define HDRP(bp) ((char *)(bp) - WSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block pointer bp, computer the address of its previous and next blocks */
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)))
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE((char *)(bp) - DSIZE))


/* &begin func definition */
static void *extend_heap(size_t);
static void *coalesce(void *);
static void *find_fit(size_t);
static void *Find_fit(size_t);
static void place(void *, size_t);
static void *Mem_sbrk(size_t);
void mm_free(void *);
void *mm_malloc(size_t);
/* &end func definition */

static char *heap_listp;
static int verbose = 0;

void *Extend_heap(size_t words) {
	void *rc;
	if ((rc = extend_heap(words)) == NULL) {
		printf("extend_heap error\n");
		exit(0);
	}
	return rc;
}

int mm_init() {
	heap_listp = Mem_sbrk(4*WSIZE);

	PUT(heap_listp, 0);
	PUT(heap_listp + WSIZE, PACK(8, 1));
	PUT(heap_listp + 2*WSIZE, PACK(8, 1));
	PUT(heap_listp + 3*WSIZE, PACK(0, 1));

	heap_listp = Extend_heap(CHUNKSIZE/WSIZE);
	return 0;
}

void *Mem_sbrk(size_t size) { return mem_sbrk(size); }

static void *extend_heap(size_t words) {
	void *bp;
	size_t size;
	
	size = (words % 2) ?(words + 1) * WSIZE : (words) * WSIZE;
	bp = Mem_sbrk(size);
	PUT((char *)bp - WSIZE, PACK(size, 0));
	PUT(FTRP(bp), PACK(size, 0));
	PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));

	return coalesce(bp);
}

void mm_free(void *bp) {
	size_t size = GET_SIZE(HDRP(bp));
	PUT(HDRP(bp), PACK(size, 0));
	PUT(FTRP(bp), PACK(size, 0));
	coalesce(bp);
}

static void *coalesce(void *bp) {
	unsigned int prev_allocated, next_allocated;	
	size_t new_size = GET_SIZE(HDRP(bp));

	prev_allocated = GET_ALLOC(HDRP(PREV_BLKP(bp)));
	next_allocated = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
	
	if (prev_allocated && next_allocated) {
		return bp;
	}

	if (prev_allocated && !next_allocated) {
		new_size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
		PUT(HDRP(bp), PACK(new_size, 0));
		PUT(FTRP(bp), PACK(new_size, 0));
		return bp;
	}
	
	if (!prev_allocated && next_allocated) {
		new_size += GET_SIZE(HDRP(PREV_BLKP(bp)));
		bp = PREV_BLKP(bp);
		PUT(HDRP(bp), PACK(new_size, 0));
		PUT(FTRP(bp), PACK(new_size, 0));
		return bp;
	}
	
	if (!prev_allocated && !next_allocated) {
		new_size += GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(HDRP(NEXT_BLKP(bp)));
		bp = PREV_BLKP(bp);
		PUT(HDRP(bp), PACK(new_size, 0));
		PUT(FTRP(bp), PACK(new_size, 0));
		return bp;
	}	

	printf("control should never reach here\n");
	exit(0);
	return bp;
}

void *mm_malloc(size_t size) {
	size_t asize;	
	void *bp;

	asize = ((size + 3*DSIZE - 1) / DSIZE) * DSIZE;
	bp = Find_fit(asize);
	place(bp, asize);
	
	return bp;
}

static void *Find_fit(size_t asize) {
	void *rc;
	if ((rc = find_fit(asize)) == NULL) {
		return Extend_heap(MAX(CHUNKSIZE, asize)/WSIZE);
	}
	return rc;
}

static void *find_fit(size_t asize) {
	void *bp;
	for (bp = heap_listp; GET_SIZE(HDRP(bp)) != 0; bp = NEXT_BLKP(bp)) {
		if (!GET_ALLOC(HDRP(bp)) && GET_SIZE(HDRP(bp)) >= asize) {
			return bp;
		}
	}
	return NULL;
}

static void place(void *bp, size_t asize) {
	size_t size = GET_SIZE(HDRP(bp));
	if (size - asize >= 4*WSIZE) {
		PUT(HDRP(bp), PACK(asize, 1));
		PUT(FTRP(bp), PACK(asize, 1));
		bp = NEXT_BLKP(bp);
		PUT(HDRP(bp), PACK(size - asize, 0));
		PUT(FTRP(bp), PACK(size - asize, 0));
	} else {
		PUT(HDRP(bp), PACK(size, 1));
		PUT(FTRP(bp), PACK(size, 1));
	}
}

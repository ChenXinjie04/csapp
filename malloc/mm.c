#include "memlib.h"
#include <stddef.h>
#include <stdlib.h>

/* Basic constants and macros */
#define WSIZE 4
#define DSIZE 8
#define CHUNKSIZE (1 << 12)

#define MAX(x, y) ((x) > (y)) ? (x) : (y)

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc) ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p) (*(unsigned int*)(p))
#define PUT(p, val) (*(unsigned int*)(p) = val)

/* Read the size and alloc fields at address p */
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x7)

/* Given block pointer bp, compute the address of its header and footer */
#define HDRP(bp) ((char *)(bp) - WSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block pointer bp, computer the address of its previous and next blocks */
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)))
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE((char *)(bp) - DSIZE))

static void *extend_heap(size_t);
static void *coalesce(void *);
static void *find_fit(size_t);
static void place(void *, size_t);
void mm_free(void *);
void *mm_malloc(size_t);

static char *heap_listp;

int mm_init() {
	if ((heap_listp = mem_sbrk(4*WSIZE)) == (void *)-1) {
		return -1;
	}

	PUT(heap_listp, 0);
	PUT(heap_listp + WSIZE, PACK(DSIZE, 1));
	PUT(heap_listp + 2*WSIZE, PACK(DSIZE, 1));
	PUT(heap_listp + 3*WSIZE, PACK(0, 1));
	heap_listp += 2*WSIZE;

	if (extend_heap(CHUNKSIZE/WSIZE) == NULL) {
		return -1;
	}

	return 0;
}

static void *extend_heap(size_t words) {
	char *bp;
	size_t size;

	size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;

	if ((bp = mem_sbrk(size)) == (void *)-1) {
		return NULL;
	}

	PUT(HDRP(bp), PACK(size, 1));
	PUT(FTRP(bp), PACK(size, 1));
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
	size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
	size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
	size_t size = GET_SIZE(HDRP(bp));

	if (prev_alloc && next_alloc) {
		return bp;
	}
	
	else if (prev_alloc && !next_alloc) {
		size += GET_SIZE(HDRP(NEXT_BLKP(bp)));

		PUT(HDRP(bp), PACK(size, 0));
		PUT(FTRP(bp), PACK(size, 0));
	}

	else if (!prev_alloc && next_alloc) {
		size += GET_SIZE(HDRP(PREV_BLKP(bp)));

		PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
		PUT(FTRP(bp), PACK(size, 0));
	}

	else {
		size += GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(HDRP(NEXT_BLKP(bp)));

		PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
		PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
		bp = PREV_BLKP(bp);
	}

	return bp;
}

void *mm_malloc(size_t size) {
	size_t asize;
	size_t extendsize;
	char *bp;

	if (size == 0) {
		return NULL;
	}

	if (size < DSIZE) {
		asize = DSIZE;
	} else {
		asize = DSIZE * ((size + (DSIZE-1))/DSIZE);
	}

	if ((bp = find_fit(asize)) != NULL) {
		place(bp, asize);
		return bp;
	}

	extendsize = MAX(asize, CHUNKSIZE);
	if ((bp = extend_heap(extendsize/WSIZE)) == NULL) {
		return NULL;
	}
	place(bp, asize);
	return bp;
}

static void *find_fit(size_t asize) {
	void *bp = heap_listp;

	size_t size = GET_SIZE(HDRP(bp));
	while (size != 0) {
		if ((size - 8) >= asize) {
			break;
		} 
		bp = NEXT_BLKP(bp);
		size = GET_SIZE(HDRP(bp));
	}

	return bp;
}

static void place(void *bp, size_t asize) {
	size_t size = GET_SIZE(HDRP(bp));
	size_t used_size = asize + 2;
	size_t remain_size = size - used_size;

	PUT(HDRP(bp), PACK(used_size, 1));
	PUT(FTRP(bp), PACK(used_size, 1));
	PUT(HDRP(NEXT_BLKP(bp)), PACK(remain_size, 0));
	PUT(FTRP(NEXT_BLKP(bp)), PACK(remain_size, 0));
}

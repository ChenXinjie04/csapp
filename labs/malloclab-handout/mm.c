/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "ateam",
    /* First member's full name */
    "Harry Bovik",
    /* First member's email address */
    "bovik@cs.cmu.edu",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8
#define WSIZE 4
#define DSIZE 8
#define CHUNCKSIZE (1 << 12)

#define MAX(x, y) ((x) > (y)) ? (x) : (y)

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define PACK(size, alloc) ((size) | (alloc))
#define PUT(p, val) (*(unsigned int *)(p) = (val))

#define GET_SIZE(p) (*(unsigned int *)(p) & ~0x7)
#define GET_ALLOC(p) (*(unsigned int *)(p) & 0x1)

#define HDRP(bp) ((char *)(bp) - WSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)))
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE((unsigned *)(bp) - DSIZE))

#define VERBOSE(msg, ...) \
    {\
        if (verbose) \
            printf(msg, ##__VA_ARGS__);\
    }\

/* &begin scalar declaration */
static char * heap_listp;
static int verbose = 1;
/* &end scalar declaration */

/* &begin func declaration */
static void *Mem_sbrk(int incr);
static void *extend_heap(size_t words);
static void *Extend_heap(size_t words);
static void place(void *bp, size_t asize);
static void printHeap(void);
static void *find_fit(size_t asize);
static void *Find_fit(size_t asize);
/* &end func declaration */

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    VERBOSE("mm_init: entering\n");
    heap_listp = Mem_sbrk(DSIZE);
    
    PUT(heap_listp, PACK(0, 1));
    PUT(heap_listp + 1, PACK(0, 1));

    if ((heap_listp = extend_heap(CHUNCKSIZE/WSIZE)) == (void *)-1) {
        return -1;
    }

    VERBOSE("mm_init: exiting\n");
    return 1;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    VERBOSE("mm_malloc: entering\n");
    size_t asize;
    void *bp;

    if (size <= 0) {
        printf("mm_malloc: invalid size (%u)\n", size);
        exit(0);
    }
    
    asize = (size + 2*DSIZE - 1) & ~0x7;
    bp = Find_fit(asize);

    place(bp, asize);
    VERBOSE("mm_malloc: alloc a size (%u) block success\n", asize);
    VERBOSE("mm_malloc: exiting\n");
    return NULL;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;
    
    newptr = mm_malloc(size);
    if (newptr == NULL)
      return NULL;
    copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
    if (size < copySize)
      copySize = size;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;
}

/*
 * extend_heap - Extend heap by (size) words.
 * RETURN: a point to the start of newly allocated block.
 */
static void *extend_heap(size_t words) {
    char *bp;
    size_t size;
    
    size = (words % 2) ? (words+1)*WSIZE : words*WSIZE;
    bp = Mem_sbrk(size);

    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));

    return bp;
}

/*
 * place - Mark a free block allocated.
 */
void place(void *bp, size_t asize) {
    size_t size;

    size = GET_SIZE(HDRP(bp));
    if (size - asize > 2*DSIZE) {
        PUT(HDRP(bp), PACK(asize, 1));
        PUT(FTRP(bp), PACK(asize, 1));
        bp = NEXT_BLKP(bp);
        PUT(HDRP(bp), PACK(size - asize, 0));
        PUT(FTRP(bp), PACK(size - asize, 0));
    } else {
        PUT(HDRP(bp), PACK(size, 1));
        PUT(FTRP(bp), PACK(size, 1));
    }

    return;
}

/*************************
 * Mem function wrappers
 *************************/

void *Mem_sbrk(int incr) { return mem_sbrk(incr); }

/*
 * Find_fit - A wrapper of find_fit, if find_fit fail it will extend heap;
 * RETURN: void.
 */
void *Find_fit(size_t asize) {
    void *rc;
    if ((rc = find_fit(asize)) == NULL) {
        size_t words = MAX(CHUNCKSIZE, asize) / WSIZE;
        return extend_heap(words);
    }
    return rc;
}

/*****************************
 * Mem check function
 *****************************/
void printHeap(void) {
    void *bp;
    for (bp = heap_listp; GET_SIZE(HDRP(bp)) != 0; bp = NEXT_BLKP(bp)) {
        printf("size=%d alloc=%d\n", GET_SIZE(HDRP(bp)), GET_ALLOC(HDRP(bp)));
    }
    return;
}

/*
 * find_fit - Find a free block at least asize large.
 * RETURN: a (void *) pointer to the beginning of payload, NULL on fail.
 */
void *find_fit(size_t asize) {
    void *bp;

    for (bp = heap_listp; GET_SIZE(HDRP(bp)) != 0; bp = NEXT_BLKP(bp)) {
        if (!GET_ALLOC(HDRP(bp)) && GET_SIZE(HDRP(bp)) >= asize) {
            return bp;
        }
    }

    return NULL;
}


/* test client */
int main() {
    mem_init();
    mm_init();
    printHeap();
    mm_malloc(10);
    printHeap();
    mm_malloc(5);
    printHeap();
    mm_malloc(17);
    printHeap();
    return 0;
}

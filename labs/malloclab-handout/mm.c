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
#include <string.h>
#include <unistd.h>

#include "memlib.h"
#include "mm.h"

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
    ""};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8
#define WSIZE 4
#define DSIZE 8
#define NFREELIST 11
#define MIN_BLK_SIZE 4
#define CHUNCKSIZE (1 << 12)

#define MAX(x, y) ((x) > (y)) ? (x) : (y)

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define PACK(size, alloc) ((size) | (alloc))
#define PUT(p, val) (*(unsigned int *)(p) = (val))

#define GET_SIZE(p) (*(unsigned int *)(p) & ~0x7)
#define GET_ALLOC(p) (*(unsigned int *)(p) & 0x1)
#define GET_P(p) ((void *)(*(unsigned *)(p)))

#define HDRP(bp) ((char *)(bp) - WSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)
#define PREVP(bp) ((char *)(bp) + WSIZE)
#define FREE_LISTP(bp, offset) ((char *)(bp) + (offset) * WSIZE)

#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)))
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE((char *)(bp) - DSIZE))

#define VERBOSE(msg, ...)                                                      \
  {                                                                            \
    if (verbose)                                                               \
      printf(msg, ##__VA_ARGS__);                                              \
  }

/* &begin scalar declaration */
static char *heap_listp;
static int verbose = 1;
static void *pre_bp;
/* &end scalar declaration */

/* &begin func declaration */
static void *extend_heap(size_t words);
static void place(void *bp, size_t asize);
static void printHeap(void);
static void *find_fit(size_t asize, int idx);
static void *coalesce(void *);
static void insertBlock(void *, size_t);
static void removeBlock(void *, void *);
static int freeListIndex(size_t);
static void printFreeList(void);
static void printOneChain(void *);
/* &end func declaration */

/*
 * mm_init - initialize the malloc package.
 */
int mm_init(void) {
  VERBOSE("mm_init: entering\n");
  void *bp;
  if ((heap_listp = mem_sbrk(NFREELIST * WSIZE + 3 * WSIZE)) == (void *)-1) {
    printf("mm_init error\n");
    exit(0);
  }

  for (int i = 0; i < NFREELIST; ++i) { /* Init free list */
    PUT(heap_listp + i * WSIZE, 0);
  }

  PUT(heap_listp + NFREELIST * WSIZE, PACK(8, 1));
  PUT(heap_listp + (NFREELIST + 1) * WSIZE, PACK(8, 1));
  PUT(heap_listp + (NFREELIST + 2) * WSIZE, PACK(0, 1));

  if ((bp = extend_heap(CHUNCKSIZE / WSIZE)) == (void *)-1) {
    return -1;
  }
  insertBlock(bp, CHUNCKSIZE);
  printFreeList();
  pre_bp = NULL; /* Temporarily block */

  VERBOSE("mm_init: exiting\n");
  return 1;
}

/*
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size) {
  VERBOSE("mm_malloc: entering\n");
  size_t asize;
  void *bp;

  if (size == 0) {
    printf("mm_malloc: invalid size (%zu)\n", size);
    exit(0);
  }

  asize = (size + 2 * DSIZE - 1) & ~0x7;
  int idx = freeListIndex(asize);
  while (idx <= 10 && bp == NULL) { /* Search the free list */
    if (idx == 10) {
      size_t words = MAX(CHUNCKSIZE, asize) / WSIZE;
      if ((bp = extend_heap(words)) == (void *)-1) {
        return NULL;
      }
    } else {
      bp = find_fit(asize, idx);
    }
    idx += 1;
  }

  place(bp, asize);
  VERBOSE("mm_malloc: alloc a size (%zu) block success\n", asize);
  if (verbose)
    printHeap();
  VERBOSE("mm_malloc: exiting\n");
  return bp;
}

/*
 * mm_free - Freeing a block does nothing.
 */
/* &begin mm_free */
void mm_free(void *ptr) {
  VERBOSE("mm_free: entering\n");
  size_t size;

  size = GET_SIZE(HDRP(ptr));

  PUT(HDRP(ptr), PACK(size, 0));
  PUT(FTRP(ptr), PACK(size, 0));
  VERBOSE("mm_free: free block success\n");
  coalesce(ptr);
  if (verbose)
    printHeap();
  VERBOSE("mm_free: exiting\n");
}
/* &end mm_free */

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size) {
  void *oldptr = ptr;
  void *newptr;
  size_t copySize;

  newptr = mm_malloc(size);
  if (newptr == NULL)
    return NULL;
  copySize = GET_SIZE(HDRP(oldptr)) - 8;
  size_t allocatedSize = GET_SIZE(HDRP(newptr)) - 8;
  if (allocatedSize < copySize)
    copySize = allocatedSize;
  memcpy(newptr, oldptr, copySize);
  mm_free(oldptr);
  return newptr;
}

/*
 * extend_heap - Extend heap by (size) words.
 * RETURN: a point to the start of newly allocated block.
 */
static void *extend_heap(size_t words) {
  VERBOSE("extend_heap: entering\n");
  char *bp;
  size_t size;

  size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;
  if ((bp = mem_sbrk(size)) == (void *)-1) {
    VERBOSE("extend_heap: no enough memory\n");
    return NULL;
  }

  PUT(HDRP(bp), PACK(size, 0));
  PUT(FTRP(bp), PACK(size, 0));
  PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));

  VERBOSE("extend_heap: exiting\n");
  return bp;
}

/*
 * place - Mark a free block allocated.
 */
void place(void *bp, size_t asize) {
  size_t size;

  size = GET_SIZE(HDRP(bp));
  if (size - asize > MIN_BLK_SIZE) {
    PUT(HDRP(bp), PACK(asize, 1));
    PUT(FTRP(bp), PACK(asize, 1));
    bp = NEXT_BLKP(bp);
    PUT(HDRP(bp), PACK(size - asize, 0));
    PUT(FTRP(bp), PACK(size - asize, 0));
    insertBlock(bp, size - asize);
  } else {
    PUT(HDRP(bp), PACK(size, 1));
    PUT(FTRP(bp), PACK(size, 1));
  }

  return;
}

/*
 * coalesce - Integrate neighbor free blocks.
 * RETURN: void
 */
/* &begin coalesce */
void *coalesce(void *bp) {
  VERBOSE("coalesce: entering\n");
  int prev_alloc, next_alloc;
  size_t size;

  prev_alloc = GET_ALLOC(HDRP(PREV_BLKP(bp)));
  next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
  size = GET_SIZE(HDRP(bp));

  if (prev_alloc && next_alloc) {
    VERBOSE("coalesce: hit cond 1\n");
    return bp;
  }

  else if (prev_alloc && !next_alloc) {
    VERBOSE("coalesce: hit cond 2\n");
    size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
  }

  else if (!prev_alloc && next_alloc) {
    VERBOSE("coalesce: hit cond 3\n");
    bp = PREV_BLKP(bp);
    size += GET_SIZE(HDRP(bp));
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
  }

  else if (!prev_alloc && !next_alloc) {
    VERBOSE("coalesce: hit cond 4\n");
    size += GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(HDRP(NEXT_BLKP(bp)));
    bp = PREV_BLKP(bp);
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
  }

  VERBOSE("coalesce: exiting\n");
  return bp;
}
/* &end coalesce */

/*************************
 * Mem function wrappers
 *************************/

/*****************************
 * Mem check function
 *****************************/
void printHeap(void) {
  VERBOSE("printHeap: entering\n");
  void *bp;
  for (bp = heap_listp; GET_SIZE(HDRP(bp)) != 0; bp = NEXT_BLKP(bp)) {
    printf("size=%d alloc=%d\n", GET_SIZE(HDRP(bp)), GET_ALLOC(HDRP(bp)));
  }
  VERBOSE("printHeap: exiting\n");
  return;
}

void printFreeList(void) {
  VERBOSE("printFreeList: entering\n");
  for (int i = 0; i < NFREELIST; ++i) {
    void *freep = FREE_LISTP(heap_listp, i);
    printf("printf chain %d\n", i);
    printOneChain(freep);
  }
  VERBOSE("printFreeList: exiting\n");
}

void printOneChain(void *freep) {
  VERBOSE("printOneChain: entering\n");
  void *bp = (void *)GET_P(freep);
  while (bp != NULL) {
    if (GET_ALLOC(HDRP(bp))) {
      printf("printOneChain: allocated block in free list\n");
      exit(0);
    }
    printf("block size (%d)\n", GET_SIZE(HDRP(bp)));
    bp = (void *)GET_P(bp);
  }
  VERBOSE("printOneChain: exiting\n");
}

/*
 * find_fit - Find a free block at least asize large.
 * RETURN: a (void *) pointer to the beginning of payload, NULL on fail.
 */
void *find_fit(size_t asize, int idx) {
  VERBOSE("find_fit: entering\n");
  void *bp;
  for (bp = FREE_LISTP(heap_listp, idx); bp != NULL; bp = (void *)GET_P(bp)) {
    if (GET_SIZE(bp) >= asize) {
      VERBOSE("find_fit: exiting with NULL\n");
      return bp;
    }
  }
  VERBOSE("find_fit: exiting with NULL\n");
  return NULL;
}

/*
 * insertBlock - Insert a block of size bytes into corresponding free list.
 * RETURN: void.
 */
void insertBlock(void *bp, size_t size) {
  VERBOSE("insertBlock: entering\n");
  if (size < 4) {
    printf("too small\n");
    exit(0);
  }
  int idx = freeListIndex(size);
  void *freep = FREE_LISTP(heap_listp, idx);

  PUT(bp, (unsigned int)GET_P(freep));
  PUT(freep, (unsigned int)bp);
  PUT(PREVP(bp), 0);
  if (GET_P(bp) != 0) {
    PUT(PREVP(GET_P(bp)), (unsigned int)bp);
  }
  VERBOSE("insertBlock: insert a free block of size (%zu) into free list (%d)\n",
          size, idx);
  VERBOSE("insertBlock: exiting\n");
}

/*
 * removeBlock - Remove a block by block pointer.
 * RETURN: a pointer to the payload.
 */
void removeBlock(void *freep, void *bp) {
  VERBOSE("removeBlock: entering\n");
  void *prev_p, *next_p;
  prev_p = (GET_P(PREVP(bp)) == NULL) ? freep : GET_P(PREVP(bp));
  PUT(prev_p, (unsigned int)GET_P(bp));
  if (GET_P(bp) != NULL) {
    VERBOSE("removeBlock: next exsist\n");
    next_p = GET_P(bp);
    PUT(PREVP(next_p), (unsigned int)GET_P(PREVP(bp)));
  }
  VERBOSE("removeBlock: exiting\n");
}

int freeListIndex(size_t size) {
  if (size <= 4) {
    return 0;
  } else if (size <= 8) {
    return 1;
  } else if (size <= 16) {
    return 2;
  } else if (size <= 32) {
    return 3;
  } else if (size <= 64) {
    return 4;
  } else if (size <= 128) {
    return 5;
  } else if (size <= 256) {
    return 6;
  } else if (size <= 512) {
    return 7;
  } else if (size <= 1024) {
    return 8;
  } else if (size <= 2048) {
    return 9;
  } else {
    return 10;
  }
}

/* test client */
int main() {
  mem_init();
  mm_init();
  removeBlock(FREE_LISTP(heap_listp, 10), GET_P(FREE_LISTP(heap_listp, 10)));
  printFreeList();
  return 0;
}

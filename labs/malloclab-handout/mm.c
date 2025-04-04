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
#include <assert.h>
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
#define NFREELIST 13
#define MIN_BLK_SIZE 16
#define CHUNCKSIZE (1 << 8)

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
static char *block_listp;
static int verbose = 0;
static void *pre_bp;
/* &end scalar declaration */

/* &begin func declaration */
static void *extend_heap(size_t words);
static void place(void *bp, size_t asize);
static void printHeap(void);
static void *find_fit(size_t asize, int idx);
static void *coalesce(void *);
static void insertBlock(void *);
static void removeBlock(void *);
static int freeListIndex(size_t);
static void printFreeList(void);
static void printOneChain(void *);
static void checkFreeList();
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

  if ((bp = extend_heap((1 << 10) / WSIZE)) == (void *)-1) {
    return -1;
  }
  pre_bp = NULL; /* Temporarily block */
  block_listp = bp;

  insertBlock(bp);
  if (verbose) {
    printHeap();
    printFreeList();
  }
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
  void *bp = NULL;

  if (size == 0) {
    printf("mm_malloc: invalid size (%zu)\n", size);
    exit(0);
  }

  asize = (size + 2 * DSIZE - 1) & ~0x7;
  int idx = freeListIndex(asize);
  while (idx <= NFREELIST && bp == NULL) { /* Search the free list */
    if (idx == NFREELIST) {
      VERBOSE("mm_malloc: extend heap\n");
      size_t words = MAX(CHUNCKSIZE, asize) / WSIZE;
      if ((bp = extend_heap(words)) == (void *)-1) {
        return NULL;
      }
    } else {
      bp = find_fit(asize, idx);
    }
    idx += 1;
  }
  VERBOSE("mm_malloc: find a suitable block in freelist (%d)\n", idx-1);

  place(bp, asize);
  if (verbose) {
    printHeap();
    printFreeList();
  }
  VERBOSE("mm_malloc: alloc a size (%zu) block success\n", asize);
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
  VERBOSE("mm_free: free block of size (%zu) success\n", size);
  coalesce(ptr);
  if (verbose) {
    printHeap();
    printFreeList();
  }
  VERBOSE("mm_free: exiting\n");
}
/* &end mm_free */

int reallocCoalesce(void *ptr, size_t asize) {
  if (GET_ALLOC(HDRP(NEXT_BLKP(ptr)))) return 0;
  size_t newsize = GET_SIZE(HDRP(ptr)) + GET_SIZE(HDRP(NEXT_BLKP(ptr)));
  if ( newsize < asize ) {
    return 0;
  }
  removeBlock(NEXT_BLKP(ptr));
  PUT(HDRP(ptr), PACK(newsize, 0));
  PUT(FTRP(ptr), PACK(newsize, 0));
  place(ptr, asize);
  return 1;
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size) {
  void *oldptr = ptr;
  void *newptr;
  size_t copySize, asize, oldBlkSize;
  
  asize = (size + 2 * DSIZE - 1) & ~0x7;
  oldBlkSize = GET_SIZE(HDRP(ptr));
  if (asize == oldBlkSize)
    return ptr;
  
  if (asize < oldBlkSize) {
    place(ptr, asize); /* inplace shrink */
    return ptr;
  }
  
  if (GET_ALLOC(HDRP(NEXT_BLKP(ptr))) && !GET_SIZE(HDRP(NEXT_BLKP(ptr)))) {
    size_t extend_size = (asize - oldBlkSize) / WSIZE;
    extend_heap(extend_size);
    PUT(HDRP(ptr), PACK(extend_size*WSIZE + oldBlkSize, 0));
    PUT(FTRP(ptr), PACK(extend_size*WSIZE + oldBlkSize, 0));
    place(ptr, asize);
    return ptr;
  }

  if (reallocCoalesce(ptr, asize)) {
    return ptr;
  }

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
  VERBOSE("place: entering\n");
  size_t size;

  size = GET_SIZE(HDRP(bp));
  VERBOSE("asize=%zu size=%zu\n", asize, size);
  if (size - asize >= MIN_BLK_SIZE) {
    PUT(HDRP(bp), PACK(asize, 1));
    PUT(FTRP(bp), PACK(asize, 1));
    bp = NEXT_BLKP(bp);
    PUT(HDRP(bp), PACK(size - asize, 0));
    PUT(FTRP(bp), PACK(size - asize, 0));
    insertBlock(bp);
  } else {
    PUT(HDRP(bp), PACK(size, 1));
    PUT(FTRP(bp), PACK(size, 1));
  }

  VERBOSE("place: exiting\n");
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
  void *next_bp;

  prev_alloc = GET_ALLOC(HDRP(PREV_BLKP(bp)));
  next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
  size = GET_SIZE(HDRP(bp));

  if (prev_alloc && next_alloc) {
    VERBOSE("coalesce: hit cond 1\n");
    insertBlock(bp);
    return bp;
  }

  else if (prev_alloc && !next_alloc) {
    VERBOSE("coalesce: hit cond 2\n");
    size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
    next_bp = NEXT_BLKP(bp);
    removeBlock(next_bp);
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    insertBlock(bp);
  }

  else if (!prev_alloc && next_alloc) {
    VERBOSE("coalesce: hit cond 3\n");
    bp = PREV_BLKP(bp);
    removeBlock(bp);
    size += GET_SIZE(HDRP(bp));
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    insertBlock(bp);
  }

  else if (!prev_alloc && !next_alloc) {
    VERBOSE("coalesce: hit cond 4\n");
    next_bp = NEXT_BLKP(bp);
    removeBlock(next_bp);
    size += GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(HDRP(NEXT_BLKP(bp)));
    bp = PREV_BLKP(bp);
    removeBlock(bp);
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    insertBlock(bp);
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
void checkValidHeapAddress(void *p) {
  if (p < mem_heap_lo() || p > mem_heap_hi()) {
    printf("invalid address (%p)\n", p);
    exit(0);
  }
}

void checkOneChain(void *);
int calFreeInChain(void *);
int calFreeInImplicitList();

void checkFreeList() {
  int chainFree = 0;
  for (int i = 0; i < NFREELIST; ++i) {
    void *freep = FREE_LISTP(heap_listp, i);
    checkOneChain(freep);
    chainFree += calFreeInChain(freep);
  }
  VERBOSE("checkFreeList: chain internal valid\n");
  int implicitFree = calFreeInImplicitList();
  if (chainFree != implicitFree) {
    fflush(stdout);
    exit(0);
  }
}

int calFreeInChain(void *freep) {
  void *bp = GET_P(freep);
  int result = 0;
  while (bp != NULL) {
    result += 1;
    bp = GET_P(bp);
  }
  return result;
}

int calFreeInImplicitList() {
  VERBOSE("calFreeInImplicitList: entering\n");
  int result = 0;
  void *bp = block_listp;
  while (1) {
    if (!GET_ALLOC(HDRP(bp))) result += 1;
    VERBOSE("result update (%d)\n", result);
    bp = NEXT_BLKP(bp);
  }
  VERBOSE("calFreeInImplicitList: exiting\n");
  return result;
}

void checkOneChain(void *freep) {
  void *bp = GET_P(freep);
  while (bp != NULL) {
    if ((void *)HDRP(bp) < mem_heap_lo() || (void *)FTRP(bp) > mem_heap_hi()) {
      printf("find invalid pointer in free list\n");
      exit(0);
    }
    if (GET_ALLOC(HDRP(bp))) {
      printf("find allocated block in free list\n");
      exit(0);
    }
    bp = GET_P(bp);
  }
}

void printHeap(void) {
  VERBOSE("printHeap: entering\n");
  void *bp;
  for (bp = block_listp; GET_SIZE(HDRP(bp)) != 0; bp = NEXT_BLKP(bp)) {
    printf("\tsize=%d alloc=%d\n", GET_SIZE(HDRP(bp)), GET_ALLOC(HDRP(bp)));
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
  void *bp = (void *)GET_P(freep);
  while (bp != NULL) {
    printf("\tblock size (%d)\n", GET_SIZE(HDRP(bp)));
    bp = (void *)GET_P(bp);
  }
  printf("\tNULL\n");
}

/*
 * find_fit - Find a free block at least asize large.
 * RETURN: a (void *) pointer to the beginning of payload, NULL on fail.
 */
void *find_fit(size_t asize, int idx) {
  void *bp;
  void *freep = FREE_LISTP(heap_listp, idx);
  for (bp = GET_P(freep); bp != NULL; bp = (void *)GET_P(bp)) {
    if (GET_SIZE(HDRP(bp)) >= asize) {
      removeBlock(bp);
      return bp;
    }
  }
  return NULL;
}

/*
 * insertBlock - Insert a block of size bytes into corresponding free list.
 * RETURN: void.
 */
void insertBlock(void *bp) {
  VERBOSE("insertBlock: entering\n");
  size_t size = GET_SIZE(HDRP(bp));
  int idx = freeListIndex(size);
  void *freep = FREE_LISTP(heap_listp, idx);

  if (verbose) {
    checkValidHeapAddress(bp);
    checkValidHeapAddress(freep);
  }
  VERBOSE("address valid\n");
  PUT(bp, (unsigned int)GET_P(freep));
  PUT(freep, (unsigned int)bp);
  PUT(PREVP(bp), 0);
  if (GET_P(bp) != 0) {
    PUT(PREVP(GET_P(bp)), (unsigned int)bp);
  }
  VERBOSE("insertBlock: insert a free block of size (%zu) into free list (%d)\n",
          size, idx);

  VERBOSE("free list valid\n");
  VERBOSE("insertBlock: exiting\n");
}

/*
 * removeBlock - Remove a block by block pointer.
 * RETURN: a pointer to the payload.
 */
void removeBlock(void *bp) {
  VERBOSE("removeBlock: entering\n");
  void *prev_p, *next_p, *freep;
  int idx = freeListIndex(GET_SIZE(HDRP(bp)));

  freep = FREE_LISTP(heap_listp, idx);
  prev_p = (GET_P(PREVP(bp)) == NULL) ? freep : GET_P(PREVP(bp));
  if (verbose) {
    checkValidHeapAddress(freep);
    checkValidHeapAddress(prev_p);
  }
  PUT(prev_p, (unsigned int)GET_P(bp));

  if (GET_P(bp) != NULL) {
    VERBOSE("removeBlock: next exsist\n");
    next_p = GET_P(bp);
    PUT(PREVP(next_p), (unsigned int)GET_P(PREVP(bp)));
  }
  VERBOSE("removeBlock: exiting\n");
}

int freeListIndex(size_t size) {
  if (size <= 24)
    return 0;
  if (size <= 32)
    return 1;
  if (size <= 64)
    return 2;
  if (size <= 80)
    return 3;
  if (size <= 120)
    return 4;
  if (size <= 240)
    return 5;
  if (size <= 480)
    return 6;
  if (size <= 960)
    return 7;
  if (size <= 1920)
    return 8;
  if (size <= 3840)
    return 9;
  if (size <= 7680)
    return 10;
  if (size <= 15360)
    return 11;
  if (size <= 30720)
    return 12;

  return 12;
}


/* test client */

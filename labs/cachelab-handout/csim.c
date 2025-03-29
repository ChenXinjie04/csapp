#include "cachelab.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>

#define GET_TAG(address) ((address & tagMask)>>(blockWidth+setWidth))
#define GET_SET(address) ((address & setMask)>>blockWidth)
#define GET_SET_BASE(setNum) (cacheBase->sets+setNum)
#define GET_LINE_BASE(setNum) (GET_SET_BASE(setNum)->lines)
#define LOG(string, ...) //printf(string, ##__VA_ARGS__)

typedef struct {
    unsigned int valid;
    uint64_t tag;
    unsigned int times;
} cacheLine, *cacheLinePtr;

typedef struct {
    cacheLinePtr lines;
} cacheSet, *cacheSetPtr;

typedef struct {
    cacheSetPtr sets;
} cache, *cachePtr;

static cachePtr cacheBase;
static uint64_t numberOfSets;
static uint64_t numberOfLines;
static uint64_t numberOfBlocks;
static unsigned int blockWidth;
static unsigned int setWidth;
static uint64_t tagMask;
static uint64_t setMask;
static unsigned int hit;
static unsigned int miss;
static unsigned int eviction;
static int verbose;

static void initCache(int, int, int);
static void initCacheLines(cacheLinePtr);
static void initMask();
// static void printCache();
// static void printSet(cacheSetPtr);
// static void printLine(cacheLinePtr);
static void readData(uint64_t);
static void writeData(uint64_t);
static int isHit(uint64_t, uint64_t);
static void evict(uint64_t, uint64_t);
static void writeLine(cacheLinePtr, uint64_t);
static void update(uint64_t);

static void initCache(int s, int E, int b) {
    LOG("In initCache s=%d E=%d b=%d\n", s, E, b);
    blockWidth = b;
    setWidth = s;
    numberOfSets = 1;
    numberOfBlocks = 1;
    numberOfLines = E;
    for (int i = 0; i < b; ++i) {
        numberOfBlocks *= 2;
    }
    for (int i = 0; i < s; ++i) {
        numberOfSets *= 2;
    }

    initMask();

    cacheBase = (cachePtr) malloc(sizeof(cache));
    cacheBase->sets = (cacheSetPtr) malloc(numberOfSets*sizeof(cacheSet));
    if (cacheBase->sets == NULL) {
        printf("malloc error\n");
        exit(1);
    }

    cacheSetPtr setPtr = cacheBase->sets;
    for (uint64_t i = 0; i < numberOfSets; ++i) {
        setPtr->lines = (cacheLinePtr) malloc(E*sizeof(cacheLine));
        if (setPtr->lines == NULL) {
            printf("malloc error\n");
            exit(1);
        }
        initCacheLines(setPtr->lines);
        setPtr += 1;
    }
}

void initCacheLines(cacheLinePtr lineBase) {
    cacheLinePtr linePtr = lineBase;
    for (uint64_t i = 0; i < numberOfLines; ++i) {
        linePtr->valid = 0;
        linePtr->tag = 0;
        linePtr->times = 0;
        linePtr += 1;
    }
}

static void initMask() {
    setMask = ((1 << setWidth) - 1) << blockWidth;

    unsigned tagWidth = 32 - blockWidth - setWidth;
    tagMask = ((1 << tagWidth) - 1) << (blockWidth + setWidth);
}

// void printCache() {
//     LOG("In printCache\n");
//     printf("Valid\t\tTag\t\ttimes\n");
//     cacheSetPtr setPtr = cacheBase->sets;
//     for (int i = 0; i < numberOfSets; ++i) {
//         printSet(setPtr);
//         setPtr += 1;
//     }
// }

// void printSet(cacheSetPtr setPtr) {
//     LOG("In printSet\n");
//     cacheLinePtr linePtr = setPtr->lines;
//     for (int i = 0; i < numberOfLines; ++i) {
//         printLine(linePtr);
//         linePtr += 1;
//     }
// }

// void printLine(cacheLinePtr linePtr) {
//     LOG("In printLine\n");
//    printf("%d\t%d\t%d\n", linePtr->valid, linePtr->tag, linePtr->times);
// }

static void readData(uint64_t address) {
    LOG("In readData\n");
    uint64_t tag = GET_TAG(address);
    uint64_t setNum = GET_SET(address);
    update(setNum);

    if (isHit(tag, setNum)) {
        LOG("hit\n");
        if (verbose) {
            printf("hit ");
        }
        hit += 1;
    } else {
        LOG("miss\n");
        miss += 1;
        evict(tag, setNum);
    }
}

static void writeData(uint64_t address) {
    uint64_t tag = GET_TAG(address);
    uint64_t setNum = GET_SET(address);
    update(setNum);

    if (isHit(tag, setNum)) {
        if (verbose) {
            printf("hit ");
        }
        hit += 1;
    } else {
        miss += 1;
        evict(tag, setNum);
    }
}

static void update(uint64_t setNum) {
    cacheLinePtr linePtr = GET_LINE_BASE(setNum);
    for (uint64_t i = 0; i < numberOfLines; ++i) {
        linePtr->times -= 1;
        linePtr += 1;
    }
}

static void evict(uint64_t tag, uint64_t setNum) {
    LOG("In evict\n");
    cacheLinePtr linePtr = GET_LINE_BASE(setNum);
    unsigned int minTimes = linePtr->times;
    cacheLinePtr evictPtr = linePtr;
    
    for (uint64_t i = 0; i < numberOfLines; ++i) {
        if (linePtr->valid == 0) {
            if (verbose) printf("miss ");
            writeLine(linePtr, tag);
            return;
        }
        if (linePtr->times < minTimes) {
            minTimes = linePtr->times;
            evictPtr = linePtr;
        }
        linePtr += 1;
    }
    if (verbose) printf("miss evict ");
    eviction += 1;
    writeLine(evictPtr, tag);
}

static void writeLine(cacheLinePtr linePtr, uint64_t tag) {
    linePtr->valid = 1;
    linePtr->tag = tag;
    linePtr->times = 0x3f3f3f3f;
}

static int isHit(uint64_t tag, uint64_t setNum) {
    LOG("In isHit. tag = %d setNum = %d\n", tag, setNum);
    cacheLinePtr linePtr = GET_LINE_BASE(setNum);
    for (uint64_t i = 0; i < numberOfLines; ++i) {
        if (linePtr->tag == tag && linePtr->valid) {
            linePtr->times = 0x3f3f3f3f;
            return 1;
        }
        linePtr += 1;
    }
    return 0;
}

int main(int argc, char* argv[])
{
    int ch;
    int s, E, b;
    char op;
    unsigned int address;
    unsigned int co;
    FILE* fd;
    char *fileName;

    verbose = 0;

    while ((ch = getopt(argc, argv, "s:E:b:t:v")) != -1) {
        switch (ch) {
            case 's':
                s = atoi(optarg);
                break;
            case 'E':
                E = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                break;
            case 't':
                fileName = optarg;
                break;
            case 'v':
                verbose = 1;
                break;
            case '?':
            default:
                exit(1);
        }
    }
    fd = fopen(fileName, "r");
    if (fd == NULL) {
        printf("Open failed\n");
        exit(1);
    }
    initCache(s, E, b);

    while(fscanf(fd, " %c %x,%d", &op, &address, &co) != EOF) {
        if (verbose && op != 'I') printf("%c %x,%d ", op, address, co);
        switch(op) {
            case 'L':
                readData(address);
                break;
            case 'S':
                writeData(address);
                break;
            case 'M':
                readData(address);
                writeData(address);
                break;
            case 'I':
                continue;
            default:
                printf("invalid operation\n");
                exit(1);
        }
        if (verbose) printf("\n");
    }

    printSummary(hit, miss, eviction);
    return 0;
}

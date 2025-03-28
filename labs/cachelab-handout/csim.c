#include "cachelab.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define GET_TAG(address) ((address & tagMask)>>(blockWidth+setWidth))
#define GET_SET(address) ((address & setMask)>>blockWidth)
#define GET_SET_BASE(setNum) (cacheBase->sets+setNum)
#define GET_LINE_BASE(setNum) (GET_SET_BASE(setNum)->lines)
#define LOG(string, ...) //printf(string, ##__VA_ARGS__)

typedef struct {
    unsigned int valid;
    unsigned int tag;
    unsigned int times;
} cacheLine, *cacheLinePtr;

typedef struct {
    cacheLinePtr lines;
} cacheSet, *cacheSetPtr;

typedef struct {
    cacheSetPtr sets;
} cache, *cachePtr;

static cachePtr cacheBase;
static unsigned int numberOfSets;
static unsigned int numberOfLines;
static unsigned int numberOfBlocks;
static unsigned int blockWidth;
static unsigned int setWidth;
static unsigned int tagMask;
static unsigned int setMask;
static unsigned int hit;
static unsigned int miss;
static unsigned int eviction;

static void initCache(int, int, int);
static void initCacheLines(cacheLinePtr);
static void initMask();
static void printCache();
static void printSet(cacheSetPtr);
static void printLine(cacheLinePtr);
static void readData(unsigned int);
static void writeData(unsigned int);
static int isHit(unsigned int, unsigned int);
static void evict(unsigned int, unsigned int);
static void writeLine(cacheLinePtr, unsigned int);

static void initCache(int s, int E, int b) {
    LOG("In initCache s=%d E=%d b=%d\n", s, E, b);
    numberOfSets = s;
    numberOfLines = E;
    numberOfBlocks = b;

    initMask();

    cacheBase = (cachePtr) malloc(sizeof(cache));
    cacheBase->sets = (cacheSetPtr) malloc(s*sizeof(cacheSet));
    if (cacheBase->sets == NULL) {
        printf("malloc error\n");
        exit(1);
    }

    cacheSetPtr setPtr = cacheBase->sets;
    for (int i = 0; i < s; ++i) {
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
    for (int i = 0; i < numberOfLines; ++i) {
        linePtr->valid = 0;
        linePtr->tag = 0;
        linePtr->times = 0;
        linePtr += 1;
    }
}

static void initMask() {
    int b = numberOfBlocks;
    int s = numberOfSets;
    while (b /= 2) blockWidth += 1;

    while (s /= 2) setWidth += 1;
    setMask = ((1 << setWidth) - 1) << blockWidth;

    unsigned tagWidth = 32 - blockWidth - setWidth;
    tagMask = ((1 << tagWidth) - 1) << (blockWidth + setWidth);
}

void printCache() {
    LOG("In printCache\n");
    printf("Valid\t\tTag\t\ttimes\n");
    cacheSetPtr setPtr = cacheBase->sets;
    for (int i = 0; i < numberOfSets; ++i) {
        printSet(setPtr);
        setPtr += 1;
    }
}

void printSet(cacheSetPtr setPtr) {
    LOG("In printSet\n");
    cacheLinePtr linePtr = setPtr->lines;
    for (int i = 0; i < numberOfLines; ++i) {
        printLine(linePtr);
        linePtr += 1;
    }
}

void printLine(cacheLinePtr linePtr) {
    LOG("In printLine\n");
   printf("%d\t%d\t%d\n", linePtr->valid, linePtr->tag, linePtr->times);
}

static void readData(unsigned int address) {
    LOG("In readData\n");
    unsigned int tag = GET_TAG(address);
    unsigned int setNum = GET_SET(address);

    if (isHit(tag, setNum)) {
        LOG("hit\n");
        hit += 1;
    } else {
        LOG("miss\n");
        miss += 1;
        evict(tag, setNum);
    }
}

static void writeData(unsigned int address) {
    unsigned int tag = GET_TAG(address);
    unsigned int setNum = GET_SET(address);

    if (isHit(tag, setNum)) {
        hit += 1;
    } else {
        miss += 1;
        evict(tag, setNum);
    }
}

static void evict(unsigned int tag, unsigned int setNum) {
    LOG("In evict");
    cacheLinePtr linePtr = GET_LINE_BASE(setNum);
    unsigned int minTimes = linePtr->times;
    cacheLinePtr evictPtr = linePtr;
    
    for (int i = 0; i < numberOfLines; ++i) {
        if (linePtr->valid == 0) {
            writeLine(linePtr, tag);
            return;
        }
        if (linePtr->times < minTimes) {
            minTimes = linePtr->times;
            evictPtr = linePtr;
        }
        linePtr += 1;
    }
    eviction += 1;
    writeLine(evictPtr, tag);
}

static void writeLine(cacheLinePtr linePtr, unsigned int tag) {
    linePtr->valid = 1;
    linePtr->tag = tag;
    linePtr->times = 1;
}

static int isHit(unsigned int tag, unsigned int setNum) {
    LOG("In isHit. tag = %d setNum = %d\n", tag, setNum);
    cacheLinePtr linePtr = GET_LINE_BASE(setNum);
    for (int i = 0; i < numberOfLines; ++i) {
        if (linePtr->tag == tag && linePtr->valid) {
            linePtr->times += 1;
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
    while ((ch = getopt(argc, argv, "s:E:b:t:")) != -1) {
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
                printf("Will open the file: %s\n", optarg);
                break;
            case '?':
            default:
                exit(1);
        }
    }

    initCache(s, E, b);
    printSummary(hit, miss, eviction);
    return 0;
}

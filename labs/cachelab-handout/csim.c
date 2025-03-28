#include "cachelab.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define getTag(address) (address & tagMask)
#define getSet(address) (address & setMask)

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
static unsigned int tagMask;
static unsigned int setMask;

static void initCache(int, int);
static void initCacheLines(cacheLinePtr);
static void initMask();
static void printCache();
static void printSet(cacheSetPtr);
static void printLine(cacheLinePtr);

static void initCache(int s, int E, int b) {
    numberOfSets = s;
    numberOfLines = E;
    numberOfBlocks = b;

    initMask();

    cacheBase = (cachePtr) malloc(s*sizeof(cacheSet));
    if (cacheBase == NULL) {
        printf("malloc error\n");
        exit(1);
    }

    cacheSetPtr setPtr = (cacheSetPtr) cacheBase;
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
    unsigned int blockWidth = 0;
    while (b/2) blockWidth += 1;

    unsigned int setWidth = 0;
    while (s/2) setWidth += 1;
    setMask = ((1 << setWidth) - 1) << blockWidth;

    unsigned tagWidth = 32 - blockWidth - setWidth;
    tagMask = ((1 << tagWidth) - 1) << (blockWidth + setWidth);
}

void printCache() {
    printf("Valid\t\tTag\t\ttimes\n");
    cacheSetPtr setPtr = (cacheSetPtr) cacheBase;
    for (int i = 0; i < numberOfSets; ++i) {
        printSet(setPtr);
        setPtr += 1;
    }
}

void printSet(cacheSetPtr setBase) {
    cacheSetPtr setPtr = setBase;
    for (int i = 0; i < numberOfLines; ++i) {
        printLine(setPtr->lines);
        setPtr += 1;
    }
}

void printLine(cacheLinePtr lineBase) {
   cacheLinePtr linePtr = lineBase; 
   printf("%d\t%d\t%d\n", linePtr->valid, linePtr->tag, linePtr->times);
}

static void readData(unsigned int address) {
    unsigned tag = getTag(address);
    unsigned set = getSet(address);

    if (hit(tag, set)) {
        hit += 1;
    } else {
        miss += 1;
        eviction += 1;
        evict(tag, set);
    }
}

static void writeData(unsigned int address) {
    unsigned tag = getTag(address);
    unsigned set = getSet(address);

    if (hit(tag, set)) {
        hit += 1;
    } else {
        miss += 1;
        eviction += 1;
        evict(tag, set);
    }
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
    printCache();
    // printSummary(0, 0, 0);
    return 0;
}

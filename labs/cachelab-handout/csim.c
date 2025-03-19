#include "cachelab.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

typedef struct {
    int valid;
    int tag;
    int byteOffset;
    int times;
} cacheLine, *cacheLinePtr;

typedef struct {
    cacheLinePtr lines;
    int numberOfLines;
} cacheSet, *cacheSetPtr;

int main(int argc, char* argv[])
{
    int ch;
    int s, E, b;
    char *file;
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
    // printSummary(0, 0, 0);
    return 0;
}

#ifndef MEMORY_LINE_H
#define MEMORY_LINE_H

#include <pthread.h>
#include <stdbool.h>

// each line of memory shared
typedef struct MemoryLine 
{
    int lineNumber;
    pthread_t pid;
    bool available;
} MemoryLine;

// contains list of lines and memory shared size
typedef struct MemorySpace
{
    MemoryLine *list;
    int size;
} MemorySpace;

typedef struct MemoryBlock
{
    int size;
    int startLine;
} MemoryBlock;


// get blocks of memory from the shared memory space
MemoryBlock* getCurrentMemoryBlocks(MemoryLine* memory, int size);

#endif
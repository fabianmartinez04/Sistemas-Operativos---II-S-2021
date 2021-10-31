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


// get blocks of memory from the shared memory space (size = memory lines length)
MemoryBlock* getCurrentMemoryBlocks(MemoryLine* memory, int size);
// save thread lines in shared memory
void saveThreadLines(MemoryLine* memory, pthread_t pid, int lines, int index);
// remove thread in shared memory
void removeThreadLines(MemoryLine* memory, pthread_t pid, int memorySize);

#endif
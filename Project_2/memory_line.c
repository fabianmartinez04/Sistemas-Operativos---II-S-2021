#include "memory_line.h"

// get blocks of memory from the shared memory space
MemoryBlock* getCurrentMemoryBlocks(MemoryLine* memory, int size) {
    MemoryBlock *blocks;
    int length = 0; // size of the list of blocks
    int blockSize = 0; // size of the block
    int startLine = -1; // start line memory of the block
    for (size_t i = 0; i < size; i++)
    {
        if(memory[i].available) 
        {
            blockSize++;
            if(startLine == -1) startLine = i;
        }
        else 
        {
            // save the block
            if (blockSize > 0) {
                length++;
                blocks = realloc(blocks, sizeof(MemoryBlock)*(length));
                blocks[length-1].size = blockSize;
                blocks[length-1].startLine = startLine;
            }
            // default values
            blockSize = 0;
            startLine = -1;

        }
    }
    // set the end of the  list of block (last block contains size = -1 and startLine = -1)
    blocks = realloc(blocks, sizeof(MemoryBlock)*(length+1));
    blocks[length].startLine = -1;
    blocks[length].size = -1;
}


// set available = false to thread lines in memory shared
void saveThreadLines(MemoryLine* memory, pthread_t pid, int lines, int index) {
    for (size_t i = index; i < index+lines; i++)
    {
        memory[i].available = false;
        memory[i].pid = pid;
    }
}

// remove thread in shared memory
void removeThreadLines(MemoryLine* memory, pthread_t pid, int memorySize) {
    bool find = false;
    for (size_t i = 0; i < memorySize; i++)
    {
        if (!memory[i].available && memory[i].pid == pid) {
            find = true;
            memory[i].pid = (long unsigned int)NULL;
            memory[i].available = true;
        }
        if (find && memory[i].pid != pid)
            break;
    }   
}
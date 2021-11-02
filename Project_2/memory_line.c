#include "memory_line.h"

// get blocks of memory from the shared memory space
MemoryBlock *getCurrentMemoryBlocks(MemoryLine *memory, int size)
{
    MemoryBlock *blocks = malloc(sizeof(MemoryBlock));
    int length = 0;     // size of the list of blocks
    int blockSize = 0;  // size of the block
    int startLine = -1; // start line memory of the block
    bool lastLineBlock = false;
    for (int i = 0; i < size; i++)
    {
        if (memory[i].available)
        {
            if (i == (size - 1))
            {
                lastLineBlock = true;
            }
            blockSize++;
            if (startLine == -1)
                startLine = i;
        }
        else
        {
            // save the block
            if (blockSize > 0)
            {
                length++;
                blocks = realloc(blocks, sizeof(MemoryBlock) * (length));
                blocks[length - 1].size = blockSize;
                blocks[length - 1].startLine = startLine;
            }
            // default values
            blockSize = 0;
            startLine = -1;
        }
    }
    // save 
    if (lastLineBlock)
    {
        length++;
        blocks = realloc(blocks, sizeof(MemoryBlock) * (length));
        blocks[length - 1].size = blockSize;
        blocks[length - 1].startLine = startLine;
    }
    
    // set the end of the  list of block (last block contains size = -1 and startLine = -1)
    blocks = realloc(blocks, sizeof(MemoryBlock) * (length + 1));
    blocks[length].startLine = -1;
    blocks[length].size = -1;
    // return block list
    return blocks;
}

// set available = false to thread lines in memory shared
void saveThreadLines(MemoryLine *memory, pthread_t pid, int lines, int index)
{
    for (size_t i = index; i < index + lines; i++)
    {
        memory[i].available = false;
        memory[i].pid = pid;
    }
}

// remove thread in shared memory
void removeThreadLines(MemoryLine *memory, Thread *t, int memorySize, FILE *binnacle)
{
    bool find = false;
    time_t ttime = time(NULL);
    int i;
    for (i = 0; i < memorySize; i++)
    {
        if (!memory[i].available && memory[i].pid == t->pid)
        {
            find = true;
            memory[i].pid = (pthread_t)NULL;
            memory[i].available = true;
        }
        else if (find && memory[i].pid != t->pid)
            break;
    }
    char dWrite[150];
    char dateData[50];
    struct tm tm = *localtime(&ttime);
    sprintf(dateData, "now: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    sprintf(dWrite, "%ld\t\t\t\t %d\t\t\t\t %s\t\t\t\t %s\t\t\t\t %s\t\t\t\t %d-%d\n", t->pid, t->lines, "Desasignacion,", "Liberar espacio", dateData, (i - t->lines), i);
    fputs(dWrite, binnacle);
}
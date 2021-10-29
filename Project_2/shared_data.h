#ifndef SHARED_DATA_H
#define SHARED_DATA_H

// libs
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>


#include <sys/shm.h> // Share memory with shmget
#include <sys/ipc.h> // Generate ipc key to use with shmget
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h> 


// data of each thread
typedef struct Thread
{
    bool alive;
    bool dead;
    bool blocked;
    pthread_t pid;
    int lines;
    int time;
} Thread;


typedef struct sharedData
{
    int linesMemorySize;
    int threadsSize;
    pthread_t pidExecution;
} sharedData;

//append element at end of the list
Thread* addThread(Thread *list, sharedData *data, int lineSize, int executeTime);

#endif
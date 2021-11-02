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

#include "defines_values.h"


// data of each thread
typedef struct Thread
{
    bool empty;
    bool alive;
    bool blocked;
    pthread_t pid;
    int lines;
    int time;
} Thread;


typedef struct sharedData
{
    bool finishFlg;
    int linesMemorySize;
    int threadsSize;
    pthread_t pidExecution;
} sharedData;

//add element to the threads list and return index
int addThread(Thread *list, sharedData *data, int lineSize, int executeTime);
// remove element to the threads list
int removeThread(Thread *list, sharedData *data, Thread *t);

#endif
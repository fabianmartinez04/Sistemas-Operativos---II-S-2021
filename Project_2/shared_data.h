#ifndef SHARED_DATA_H
#define SHARED_DATA_H

// libs
#include <pthread.h>
#include <stdbool.h>

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

// init list struct
void initList(sharedData *);
//append element at end of the list
void addThread(sharedData *, Thread*);

#endif
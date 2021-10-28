#ifndef SHARED_DATA_H
#define SHARED_DATA_H

// libs
#include <pthread.h>
#include <stdbool.h>

// data of each thread
typedef struct Thread
{
    bool alive;
    pthread_t pid;
    int lines;
    int time;
} Thread;


typedef struct sharedData
{
    int linesMemorySize;
    Thread * list;
    int listSize;
} sharedData;

// init list struct
void initList(sharedData *);
//append element at end of the list
void addThread(sharedData *, Thread*);

#endif
#ifndef THREAD_LIST_H
#define THREAD_LIST_H

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


typedef struct List
{
    Thread * list;
    int size;
} List;

// init list struct
void initList(List *);
//append element at end of the list
void addThread(List *, Thread*);

#endif
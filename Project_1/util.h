#ifndef UTIL_H
#define UTIL_H

//includes
#include <pthread.h> // thread library
#include <stdlib.h>
#include <stdio.h>

// struct: Step
// save each step of the thread or fork in the labyrith
typedef struct Step
{
    int row;
    int column;
    struct Step *next;
}Step;

// struct: Thread
// save all steps that do each thread in the matrix
typedef struct Thread{
    // int id;
    pthread_t *currentThread;
    Step *steps;
    int direction;
    struct Thread *next;
} Thread;

Step *getLastStep(Thread *thread);

void addElementAtEnd(Thread *list, Thread *item);

Thread *createNewThread(int row, int column, int direction);


#endif /*UTIL_H*/
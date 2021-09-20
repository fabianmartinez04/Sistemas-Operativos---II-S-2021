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
typedef struct Thread
{
    pthread_t *currentThread;
    Step *steps;
    int direction;
    struct Thread *next;
} Thread;


// struct Step of the fork
typedef struct
{
    int row;
    int column;
}StepFork;

// struct: Fork
// contain all information of the fork
typedef struct Fork {
    pid_t pid;
    int direction;
    int stepN;
    StepFork *steps[];
} Fork;

Step *getLastStepThread(Thread *thread);

void addThreadAtEnd(Thread *list, Thread *item);

Thread *createNewThread(int row, int column, int direction);

// get fork by pid_t
Fork *getFork(Fork *list, pid_t pid, int forkLenght);

// add fork at the end of the list
//void addForkAtEnd(Fork *list[], Fork *item, int lenght);

// create new fork
void createNewFork(int row, int column, int direction, Fork *list, int lenght);

// get last step fork
//Step *getLastStepFork(Fork *fork);
#endif /*UTIL_H*/
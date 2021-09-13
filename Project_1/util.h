#ifndef UTIL_H
#define UTIL_H

//includes
#include <pthread.h> // thread library

// struct: Step
// save each step of the thread or fork in the labyrith
typedef struct Step
{
    int row;
    int column;
    struct Step *next;
}Step;

// struct Steps
typedef struct {
    int n; // total of steps
    Step *firstStep;
} Steps;

// struct: Thread
// save all steps that do each thread in the matrix
typedef struct {
    pthread_t *thread_id;
    Steps *steps;
} Thread;

#endif /*UTIL_H*/
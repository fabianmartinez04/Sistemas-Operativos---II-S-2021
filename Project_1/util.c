#include "util.h"

#include <sys/mman.h> // shared memory
#include <unistd.h>

Step *getLastStepThread(Thread *thread) {
    // get the first step of the linked list;
    Step *aux = thread->steps;
    while (aux->next != NULL)
    {
        aux = aux->next;
    }
    return aux;
};


Step *getStepAtEnd(Step *steps) {
    Step *iter = steps;
    while (iter != NULL)
    {
        iter = iter->next;
    }
    return iter;
}

void addThreadAtEnd(Thread *list, Thread *item) {
    // get the fisrt element of the list
    Thread *aux = list;
    while(aux->next != NULL) {
        aux = aux->next;
    }
    aux->next = item;
}


Thread *createNewThread(int row, int column, int direction) {
    Thread *newItem = malloc(sizeof(Thread));
    Step *firstStep = malloc(sizeof(Step));
    *firstStep = (Step){
        .row = row,
        .column = column,
        .next = NULL
    };
    *newItem = (Thread){
        .currentThread = malloc(sizeof(pthread_t)),
        .steps = firstStep,
        .direction = direction,
        .next = NULL
    };
    return newItem;
}


// get fork by pid_t
Fork *getFork(Fork *list, pid_t pid) {
    Fork *aux = list;
    while (aux != NULL)
    {
        if (aux->pid == pid) {
            return aux;
        }
        aux = aux->next;
    }
    return NULL;
}

// add fork at the end of the list
void addForkAtEnd(Fork *list, Fork *item) {
    // get the fisrt element of the list
    Fork *aux = list;
    while(aux->next != NULL) {
        aux = aux->next;
    }
    aux->next = item;
}

// create new fork
Fork *createNewFork(int row, int column, int direction) {
    Fork *newItem = mmap(NULL, sizeof(Fork), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    Step *firstStep = mmap(NULL, sizeof(Step), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *firstStep = (Step){
        .row = row,
        .column = column,
        .next = NULL
    };
    *newItem = (Fork){
        .pid =  -1,
        .steps = firstStep,
        .direction = direction,
        .next = NULL
    };
    return newItem;
}

// get last step fork
Step *getLastStepFork(Fork *fork) {
    // get the first step of the linked list;
    Step *aux = fork->steps;
    while (aux->next != NULL)
    {
        aux = aux->next;
    }
    return aux;
}
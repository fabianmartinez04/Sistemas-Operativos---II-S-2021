#include "util.h"

#include <unistd.h>
#include <sys/mman.h> // shared memory
# define MREMAP_MAYMOVE 1
# define MREMAP_FIXED   2

Step *getLastStepThread(Thread *thread) {
    // get the first step of the linked list;
    Step *aux = thread->steps;
    while (aux->next != NULL)
    {
        aux = aux->next;
    }
    return aux;
};


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
Fork *getFork(Fork *list, pid_t pid, int forkLenght) {
    int i = 0;
    while (i < forkLenght)
    {
        if (list[i].pid == pid) {
            return &list[i];
        }
        i++;
    }
    return NULL;
}

/*
// add fork at the end of the list
void addForkAtEnd(Fork *list[], Fork *item, int lenght) {
    mremap(list, lenght * sizeof(Fork), (lenght + 1) * sizeof(Fork), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    list[lenght] = item;
}*/

// create new fork
void createNewFork(int row, int column, int direction, Fork *list, int lenght) {

    void *temp = mremap(list, lenght * sizeof(Fork), (lenght + 1) * sizeof(Fork), MREMAP_MAYMOVE | MREMAP_FIXED);
    list = temp;
    list[lenght].steps[0] = mmap(NULL, sizeof(Step), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *list[lenght].steps[0] = (StepFork){
        .row = row,
        .column = column
    };
    list[lenght] = (Fork){
        .pid =  -1,
        .direction = direction
    };
}

/*
// get last step fork
Step *getLastStepFork(Fork *fork) {
    // get the first step of the linked list;
    Step *aux = fork->steps;
    while (aux->next != NULL)
    {
        aux = aux->next;
    }
    return aux;
}*/
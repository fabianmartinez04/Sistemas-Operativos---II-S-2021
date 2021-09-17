#include "util.h"


Step *getLastStep(Thread *thread) {
    // get the first step of the linked list;
    Step *aux = thread->steps;
    while (aux->next != NULL)
    {
        aux = aux->next;
    }
    return aux;
};


void addElementAtEnd(Thread *list, Thread *item) {
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
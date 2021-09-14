#include "util.h"


Step *getLastStep(Thread *thread) {
    Step *aux = thread->moves->firstStep;
    while (aux->next == NULL)
    {
        aux = aux->next;
    }
    return aux;
};
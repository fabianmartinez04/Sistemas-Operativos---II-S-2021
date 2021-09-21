#include "util.h"

#include <sys/mman.h> // shared memory
#include <unistd.h>
#include <string.h>

Step *getLastStepThread(Thread *thread)
{
    // get the first step of the linked list;
    Step *aux = thread->steps;
    while (aux->next != NULL)
    {
        aux = aux->next;
    }
    return aux;
};

void addThreadAtEnd(Thread *list, Thread *item)
{
    // get the fisrt element of the list
    Thread *aux = list;
    while (aux->next != NULL)
    {
        aux = aux->next;
    }
    aux->next = item;
}

Thread *createNewThread(int row, int column, int direction)
{
    Thread *newItem = malloc(sizeof(Thread));
    Step *firstStep = malloc(sizeof(Step));
    *firstStep = (Step){
        .row = row,
        .column = column,
        .next = NULL};
    *newItem = (Thread){
        .currentThread = malloc(sizeof(pthread_t)),
        .steps = firstStep,
        .printed = 0,
        .direction = direction,
        .next = NULL};
    return newItem;
}

// get fork by pid_t
Fork *getFork(Fork *list, pid_t pid)
{
    Fork *aux = list;
    while (aux != NULL)
    {
        if (aux->pid == pid)
        {
            return aux;
        }
        aux = aux->next;
    }
    return NULL;
}

// add fork at the end of the list
void addForkAtEnd(Fork *list, Fork *item)
{
    // get the fisrt element of the list
    Fork *aux = list;
    while (aux->next != NULL)
    {
        aux = aux->next;
    }
    aux->next = item;
}

// create new fork
Fork *createNewFork(int row, int column, int direction)
{
    Fork *newItem = mmap(NULL, sizeof(Fork), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    Step *firstStep = mmap(NULL, sizeof(Step), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *firstStep = (Step){
        .row = row,
        .column = column,
        .next = NULL};
    *newItem = (Fork){
        .pid = -1,
        .steps = firstStep,
        .direction = direction,
        .next = NULL};
    return newItem;
}

// get last step fork
Step *getLastStepFork(Fork *fork)
{
    // get the first step of the linked list;
    Step *aux = fork->steps;
    while (aux->next != NULL)
    {
        aux = aux->next;
    }
    return aux;
}

// read fork steps from file
Fork *readForkSteps()
{
    FILE *file;
    Fork *newFork;
    Fork *list = NULL;
    char filename[20] = "steps.txt";
    //Name of the file
    file = fopen(filename, "r");
    //validate if the file exists
    if (file == NULL)
    {
        printf("File is not available \n");
    }
    else
    {
        char line[200] = "";
        char *value;
        while (fgets(line, 200, file))
        {
            value = strtok(line, " ");
            int pid = atoi(value);
            if (getFork(list, pid) == NULL)
            {
                newFork = malloc(sizeof(Fork));
                newFork->pid = pid;
                value = strtok(NULL, " ");
                newFork->direction = atoi(value);
                newFork->next = NULL;
                Step *stepAux = NULL;
                while (value)
                {
                    Step *s = malloc(sizeof(Step));
                    s->next = NULL;
                    value = strtok(NULL, " ");
                    if (value == "\n")break;
                    s->row = atoi(value);
                    value = strtok(NULL, " ");
                    if (value == NULL)break;
                    s->column = atoi(value);
                    if (stepAux == NULL)
                    {
                        newFork->steps = s;
                        stepAux = s;
                    }
                    else
                    {
                        stepAux->next = s;
                        stepAux = s;
                    }
                }
                if (list == NULL)
                {
                    list = newFork;
                } else {
                    newFork->next = list;
                    list = newFork;
                }
            }
        }
    }
    //close the file
    fclose(file);
    return list;
}


void printSteps(int id, Step *steps, char *type, bool exit) {
    printf("%s : %d ended, exit: ", type, id);
    if (exit) printf("Found, steps: ");
    else printf("No found, steps: ");
    Step *iter = steps;
    while (iter != NULL)
    {
        printf("(%d,%d)", iter->row, iter->column);
        iter = iter->next;
        if (iter != NULL) {
            printf("->");
        }
    }
    printf("\n");
}
#include "shared_data.h"

//append element at end of the list
int addThread(Thread *list, sharedData *data, int lineSize, int executeTime) {
    data->threadsSize = data->threadsSize + 1;
    int index;
    for (int i = 0; i < data->threadsSize; i++)
    {
        if(list[i].empty) {
            index = i;
            break;
        }
    }
    list[index].time = executeTime;
    list[index].lines = lineSize;
    list[index].alive = true;
    list[index].blocked = false;
    list[index].empty = false;
    return index;
}


// remove element to the threads list
int removeThread(Thread *list, sharedData *data, Thread *t) {
    for (int i = 0; i < data->threadsSize; i++)
    {
        if (list[i].pid == t->pid) {
            list[i].empty = true;
            list[i].alive = false;
            list[i].blocked = false;
            list[i].lines = 0;
            list[i].pid = -1;
            list[i].time = 0;
            break;
        }
    }
}
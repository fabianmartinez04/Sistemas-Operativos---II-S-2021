#include "shared_data.h"

//append element at end of the list
Thread* addThread(Thread *list, sharedData *data, int lineSize, int executeTime) {
    // reize a shared memory
    data->threadsSize = data->threadsSize + 1;
    key_t key = ftok("/bin/ls",22);
    list[data->threadsSize-1].time = executeTime;
    list[data->threadsSize-1].lines = lineSize;
    list[data->threadsSize-1].alive = true;
    list[data->threadsSize-1].blocked = false;
    list[data->threadsSize-1].dead = false;
    return list;
}

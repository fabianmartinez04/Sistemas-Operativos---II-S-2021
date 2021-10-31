#include "shared_data.h"

//append element at end of the list
void addThread(Thread *list, sharedData *data, int lineSize, int executeTime) {
    data->threadsSize = data->threadsSize + 1;
    list[data->threadsSize-1].time = executeTime;
    list[data->threadsSize-1].lines = lineSize;
    list[data->threadsSize-1].alive = true;
    list[data->threadsSize-1].blocked = false;
    list[data->threadsSize-1].dead = false;
}

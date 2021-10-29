// libs
#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <unistd.h>

#include "memory_line.h"
#include "shared_data.h"

#define LINE_RANGE 1,10
#define THREAD_EXECUTE_TIME 20,60
#define THREAD_TIME 30,60




// global variables
sharedData *data;
MemoryLine *lines;
Thread *threads;
int semId[2];
struct sembuf operation;

// function definition
bool bestFit(Thread*);
bool worstFit(Thread*);
bool firstFit(Thread*);
Thread *createThread();
void threadGenerate();
int getRandom(int lower, int upper);


Thread *createThread() {
    int lineSize = getRandom(LINE_RANGE);
    int executeTime = getRandom(THREAD_EXECUTE_TIME);
    threads = addThread(threads,data,lineSize,executeTime);
    printf("Thread created\n");
    return &threads[data->threadsSize-1];
}

// get a random number betwen
int getRandom(int lower, int upper){
    return (rand() %(upper - lower + 1)) + lower;
}

int main() {

    // shmget returnsan identifier in shdid (get sharedData memory id)
    int shdid = shmget(ftok("/bin/ls", 21), sizeof(sizeof(sharedData)), 0777);
    // shmat to attach to data shared
    data = (sharedData*)shmat(shdid,0,0);
    //semget returns an identifier in semKey (semaphores)
    semId[0] = semget(ftok("/bin/ls", 23), 2, 0600);
    // shmget returns an identifier in shtid (get threads memory id)
    int shtid = shmget(ftok("/bin/ls",22), data->threadsSize*sizeof(sizeof(Thread)), 0777);
    // shmat to attach to threads shared
    threads = (Thread*) shmat(shtid,0,0);
    // shmget returns an identifier in shmid (get MemoryLine memory id)
    int shmid = shmget(ftok("/bin/ls", 20),data->linesMemorySize*sizeof(MemoryLine), 0777);
    // shmat to attach to memoryLine Shared
    lines = (MemoryLine*) shmat(shmid,0,0);

    for (int i = 0; i < 5; i++)
    {
        createThread();
    }
    
    
    //detach from shared memory 
    shmdt(lines);
    shmdt(data);
    shmdt(threads);
    

    return 0;
}
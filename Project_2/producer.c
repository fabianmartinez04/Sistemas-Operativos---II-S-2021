// run gcc -o producer producer.c memory_line.c shared_data.c

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
#include "defines_values.h"




// global variables
sharedData *data;
MemoryLine *lines;
Thread *threads;
int semId[2];
struct sembuf operation;

// function definition
bool bestFit(Thread*);
bool worstFit(Thread*, MemoryBlock*);
bool firstFit(Thread*, MemoryBlock*);
Thread *createNewThread();
void threadGenerate();
int getRandom(int lower, int upper);
int algorithmOption();


bool bestFit(Thread* t) {
    int index = 0;
    int bestBlockIndex = -1;
    MemoryBlock* blocks = getCurrentMemoryBlocks(lines,data->linesMemorySize);
    while (blocks->size != -1) {
        if(blocks[index].size >= t->lines) {
            if (bestBlockIndex != -1) {
                if (blocks[index].size < blocks[bestBlockIndex].size)
                    bestBlockIndex = index;
            }
            else {
                bestBlockIndex = index;
            }
        
        }
        index++;
    }
    //save thread lines in shared memory
    if (bestBlockIndex != -1)
        saveThreadLines(lines,t->pid,t->lines,blocks[bestBlockIndex].startLine);
    // return true if thread found space in lines memory 
    return bestBlockIndex != -1;
}


bool worstFit(Thread* t, MemoryBlock* blocks) {


}

bool firstFit(Thread* t, MemoryBlock* blocks) {

}

Thread *createNewThread() {
    int lineSize = getRandom(LINE_RANGE);
    int executeTime = getRandom(THREAD_EXECUTE_TIME);
    addThread(threads,data,lineSize,executeTime);
    printf("Thread created\n");
    return &threads[data->threadsSize-1];
}

// get a random number betwen
int getRandom(int lower, int upper){
    return (rand() %(upper - lower + 1)) + lower;
}

// function to create thread every (30-60) seconds  
void threadGenerate() {
    int option = memoryOption();
    // if thread found space in memory -> threadInMemory = true
    bool threadInMemory;
    while (true)
    {
        //wait semaphore
        if (data->finishFlg)
        {
            break;
        }
        // created thread
        Thread *t = createNewThread();
        // run algorithm selected
        
        switch (option)
        {
        case  1: // First fit algorithm

            break;
        case  2: // Worst fit algorithm
        
            break;
        
        case  3: // Best fit algorithm
            threadInMemory = bestFit(t);
            break;
        }
        // create thread

        // sleep (30-60) seconds
        sleep(getRandom(THREAD_TIME));
    }
    
    // pthread_join() to wait for thread in execution

    // close binnacle
    
}

int memoryOption(){
    int option;
    printf("\t\tAllocationg memory algorithm option\n");
    printf("1. First fit algorithm\n");
    printf("2. Worst fit algorithm\n");
    printf("3. Best fit algorithm\n");
    printf("option: ");
    scanf("%d", &option);
    return option;
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
    
    
    // call function generator
    threadGenerate();

    //detach from shared memory 
    shmdt(lines);
    shmdt(data);
    shmdt(threads);
    

    return 0;
}
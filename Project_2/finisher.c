#include <sys/file.h> // File managment
#include <sys/shm.h> // Share memory with shmget
#include <sys/ipc.h> // Generate ipc key to use with shmget
#include <stdio.h>
#include <sys/types.h>
#include <sys/sem.h> // To implement semaphore
#include <stdlib.h> 
#include "memory_line.h"
#include "shared_data.h"
#define file "binnacle.txt"

// Global variables
sharedData *data;
MemoryLine *lines;
Thread *threads;
int *semId;
struct sembuf increase;
struct semuf decrease;

int main() {

    // Variables
    int i;

    // Generate unique key to shmid
    key_t shmKey = ftok("shmfile", 21);
 

    /*****************************Get shared data****************************/

    // Generate unique key to shdid
    key_t shdKey = ftok("shdfile", 21);
 
    // Identifier for shared data
    int shdid = shmget(shdKey,1*sizeof(sharedData),0777|IPC_CREAT);

    // Pointer to shared data
    data = (sharedData*) shmat(shdid,0,0);


    // Identifier for shared memory
    int shmid = shmget(shmKey,data->linesMemorySize*sizeof(MemoryLine),0777|IPC_CREAT);

    // Destroy memory
    shmdt ((sharedData *)data);
	shmctl (shmid, IPC_RMID, (struct shmid_ds *)NULL);

    //semget returns an identifier in semKey (semaphores)
    semId = semget(ftok("shsfile", 21), 2, 0600 | IPC_CREAT);

    // shmget returns an identifier in shtid (get threads memory id)
    int shtid = shmget(ftok("shtfile",21), data->threadsSize*sizeof(sizeof(Thread)), 0777|IPC_CREAT);

    // shmat to attach to threads shared
    threads = (Thread*) shmat(shtid,(void*)0,0);

    // shmget returns an identifier in shmid (get MemoryLine memory id)
    int shmid = shmget(ftok("shmfile", 21),data->linesMemorySize*sizeof(MemoryLine), 0777|IPC_CREAT);

    // shmat to attach to memoryLine Shared
    lines = (MemoryLine*) shmat(shmid,(void*)0,0);


    // Finalize each thread
    /*
    int totalThreads = data->threadsSize;
    for (i = 0; i < totalThreads; ++i){
        if (threads[i].alive){
            threads[i].pid
        }
    
    }
    */
}

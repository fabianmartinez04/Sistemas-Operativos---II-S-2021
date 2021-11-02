// run: gcc -o finisher finisher.c

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
int semId;
struct sembuf operation;

int main() {

    /*****************************GET pointers to shared memory****************************/

    // shmget returnsan identifier in shdid (get sharedData memory id)
    int shdid = shmget(ftok("/bin/ls", 21), sizeof(sizeof(sharedData)), 0777);
    // shmat to attach to data shared
    data = (sharedData*)shmat(shdid,0,0);
    //semget returns an identifier in semKey (semaphores)
    semId = semget(ftok("/bin/ls", 23), 2, 0600);
    // shmget returns an identifier in shtid (get threads memory id)
    int shtid = shmget(ftok("/bin/ls",22), data->threadsSize*sizeof(sizeof(Thread)), 0777);
    // shmat to attach to threads shared
    threads = (Thread*) shmat(shtid,0,0);
    // shmget returns an identifier in shmid (get MemoryLine memory id)
    int shmid = shmget(ftok("/bin/ls", 20),data->linesMemorySize*sizeof(MemoryLine), 0777);
    // shmat to attach to memoryLine Shared
    lines = (MemoryLine*) shmat(shmid,0,0);


    // send singal to finish to producer program
    // wait semaphore (decrese)
	operation.sem_num = 0;
	operation.sem_op = 1;
	operation.sem_flg = 0;
    semop(semId, &operation, 1);

    printf("sending flag to terminate producer program...\n");
    // send singal to finish to producer program
    data->finishFlg = true;

    // signal semaphore
    operation.sem_num = 0;
	operation.sem_op = -1;
	operation.sem_flg = 0;
    semop (semId, &operation, 1);

    // dettach 
    shmdt(lines);
    shmdt(data);
    shmdt(threads);

    operation.sem_num = 1;
	operation.sem_op = -2;
	operation.sem_flg = 0;
    printf("waiting for producer program...\n");

    semop (semId, &operation, 1);
    printf("destroying shared memory...\n");
    // destroy the shared memory // REMOVE THIS
    shmctl(shmid,IPC_RMID,NULL);
    shmctl(shdid,IPC_RMID,NULL);
    shmctl(shtid,IPC_RMID,NULL);
    // terminate semaphores
    semctl(semId,1,IPC_RMID);

}

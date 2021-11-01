// Spy program
// run command: gcc -o spy spy.c 


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

// defines
#define WIDTH 20


// global variables
sharedData *data;
MemoryLine *lines;
Thread *threads;
int semId;
struct sembuf operation;



// functions definition
void showMenu();
void memoryStatus();
void ThreadStatus();

// show the menu options
void showMenu() {
    int option;
    printf("--------------- MENU ------------\n");
    printf("1. Memory status\n");
    printf("2. Threads status\n");
    printf("Option: ");
    scanf("%d", &option);

    switch (option)
    {
    case 1:
        memoryStatus();
        break;
    case 2:
        ThreadStatus();
        break;
    default:
        break;
    }
}

//show information about shared memory
void memoryStatus() {

    
    // wait semaphore (decrese)
	operation.sem_num = 0;
	operation.sem_op = 1;
	operation.sem_flg = 0;
    semop(semId, &operation, 1);
    printf("\t\t\t MEMORY INFORMATION\n\n");
    for (int i = 0; i <= data->linesMemorySize && data->linesMemorySize!=0; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            printf("-");
        }
        if (i < data->linesMemorySize) {
            (!lines[i].available)?printf("\nline %d: %ld\n", i+1, lines[i].pid):printf("\nline %d\n", i+1);
        } else {
            printf("\n");
        }
        
    }
    // signal semaphore
    operation.sem_num = 0;
	operation.sem_op = -1;
	operation.sem_flg = 0;
    semop (semId, &operation, 1);
}

void ThreadStatus() {
    // wait a semaphore
    operation.sem_num = 0;
	operation.sem_op = 1;
	operation.sem_flg = 0;
    semop (semId, &operation, 1);
    printf("\t\t\t THREAD INFORMATION\n\n");
    printf("Memory Access PID: %ld \n\n", data->pidExecution);
    printf("PID EXECUTE RIGHT NOW:\n");
    for (int i = 0; i < data->threadsSize; i++)
    {
        printf("PID: %ld \n", threads[i].pid);
    }
    printf("\nPID BLOCK (wating for looking memory space):\n");
    for (int i = 0; i < data->threadsSize; i++)
    {
        if (threads[i].blocked) {
            printf("PID: %ld \n", threads[i].pid);
        }
    }
    operation.sem_num = 0;
	operation.sem_op = -1;
	operation.sem_flg = 0;
    semop (semId, &operation, 1);
    // signal a semaphore
    
}

int main() {

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


    showMenu();

    //detach from shared memory 

    shmdt(lines);
    shmdt(data);
    shmdt(threads);
    

    return 0;
}



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
int *semId;
struct sembuf operation;


int size = 10;


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

    printf("\t\t\t MEMORY INFORMATION\n\n");
    // wait semaphore (decrese)
	operation.sem_num = 0;
	operation.sem_op = -1;
	operation.sem_flg = 0;
    semop (semId[0], &operation, 1);
    for (int i = 0; i <= data->linesMemorySize; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            printf("-");
        }
        if (i < size) {
            printf("\nline %d: %s\n", i+1, lines[i].available?"":lines[i].pid);
        } else {
            printf("\n");
        }
        
    }
    // signal semaphore
    operation.sem_num = 0;
	operation.sem_op = 1;
	operation.sem_flg = 0;
    semop (semId[0], &operation, 1);
}

void ThreadStatus() {
    printf("\t\t\t THREAD INFORMATION\n\n");
    // wait a semaphore
    operation.sem_num = 0;
	operation.sem_op = -1;
	operation.sem_flg = 0;
    semop (semId[0], &operation, 1);
    printf("Memory Access PID: %d \n\n", data->pidExecution);
    printf("PID EXECUTE RIGHT NOW:\n");
    for (int i = 0; i < data->threadsSize; i++)
    {
        printf("PID: %d \n", threads[i].pid);
    }
    printf("\nPID BLOCK (wating for looking memory space):\n");
    for (int i = 0; i < data->threadsSize; i++)
    {
        if (threads[i]->block) {
            printf("PID: %d \n", threads[i].pid);
        }
    }
    operation.sem_num = 0;
	operation.sem_op = 1;
	operation.sem_flg = 0;
    semop (semId[0], &operation, 1);
    // signal a semaphore
    
}

int main() {

    // shmget returns an identifier in shdid (get sharedData memory id)
    int shdid = shmget(ftok("shdfile", 21), sizeof(sizeof(sharedData)), 0777|IPC_CREAT);
    // shmat to attach to data shared
    data = (sharedData*)shmat(shdid,(void*)0,0);
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

  
    // destroy the shared memory // REMOVE THIS
    shmctl(shmid,IPC_RMID,NULL);

    showMenu();
    
    //detach from shared memory 

    shmdt(lines);
    shmdt(data);
    shmdt(threads);


    return 0;
}



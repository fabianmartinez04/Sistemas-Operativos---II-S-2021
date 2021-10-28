// Spy program
// run command: gcc -o spy spy.c


// libs

#include <stdlib.h>
#include <stdio.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include "memory_line.h"
#include "sharead_data.h"

// defines
#define WIDTH 20

// global variables

key_t shmKey;
MemoryLine *lines;
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
    // wait semaphore

    for (int i = 0; i <= size; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            printf("-");
        }
        if (i < size) {
            printf("\nline %d: %s\n", i+1, 1==1?"empty":"pid");
        } else {
            printf("\n");
        }
        
    }
    // signal semaphore
    
}

void ThreadStatus() {
    printf("\t\t\t THREAD INFORMATION\n\n");
    // wait a semaphore
    printf("Memory Access PID: %d \n\n", 123456);
    printf("PID EXECUTE RIGHT NOW:\n");
    for (int i = 0; i < 5; i++)
    {
        printf("PID: %d \n", 658*(i+1));
    }
    printf("\nPID BLOCK (wating for looking memory space):\n");
    for (int i = 0; i < 5; i++)
    {
        printf("PID: %d \n", 458*(i+1));
    }

    // signal a semaphore
    
}

int main() {

    // ftok to generate unique key
    shmKey = ftok("shmfile", 21);


   // shmget returns an identifier in shmid
    int shmid = shmget(shmKey,sizeof(MemoryLine),0777|IPC_CREAT);
  
    // shmat to attach to shared memory
    lines = (MemoryLine*) shmat(shmid,(void*)0,0);
  

    //detach from shared memory 
    shmdt(lines);

    // destroy the shared memory // remove this after
    shmctl(shmid,IPC_RMID,NULL);

    
    showMenu();
    
    return 0;
}



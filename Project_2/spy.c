// Spy program
// run command: gcc -o spy spy.c


// libs

#include <stdlib.h>
#include <stdio.h>


#include <sys/ipc.h>
#include <sys/shm.h>
#include "memory_line.h"


// functions definition
int showMenu();

// show the menu options
int showMenu() {
    int option;
    printf("--------------- MENU ------------\n");
    printf("1. Memory status\n");
    printf("2. Threads status\n");
    printf("3. Exit\n");
    printf("Option: ");
    scanf("%d", &option);
    return option;
}

int main() {

    // variables
    int option;

    // ftok to generate unique key
    key_t key = ftok("shmfile",65);


   // shmget returns an identifier in shmid
    int shmid = shmget(key,sizeof(MemoryLine),0666|IPC_CREAT);
  
    // shmat to attach to shared memory
    MemoryLine *lines = (MemoryLine*) shmat(shmid,(void*)0,0);
  
    printf("Data read from memory: %d, %ld, %d\n",lines[0].available, lines[0].pid, lines[0].lineNumber);
    printf("Data read from memory: %d, %ld, %d\n",lines[1].available, lines[1].pid, lines[1].lineNumber);


    //detach from shared memory 
    shmdt(lines);

    // destroy the shared memory
    shmctl(shmid,IPC_RMID,NULL);

    /*do {
        option = showMenu();
    }while (option != 3);*/
    
    return 0;
}



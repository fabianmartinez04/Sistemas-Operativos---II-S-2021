#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include "memory_line.h"

  
int main()
{
    int size = 2;
    // ftok to generate unique key
    key_t key = ftok("shmfile",21);
  
    // shmget returns an identifier in shmid
    int shmid = shmget(key,size*sizeof(MemoryLine),0666|IPC_CREAT);
  
    // shmat to attach to shared memory
    MemoryLine *lines = (MemoryLine*) shmat(shmid,(void*)0,0);
  
    printf("Write Data : ");
    lines[0].lineNumber = 1;
    lines[0].available = true;
    lines[0].pid = 123456;
    lines[1].lineNumber = 2;
    lines[1].available = true;
    lines[1].pid = 654321;
  
    printf("Data written in memory");
      
    //detach from shared memory 
    shmdt(lines);
  
    return 0;
}
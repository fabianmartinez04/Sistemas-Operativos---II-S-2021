#include <sys/file.h> // File managment
#include <sys/shm.h> // Share memory with shmget
#include <sys/ipc.h> // Generate ipc key to use with shmget
#include <stdio.h>
#include "memory_line.h"
#define file "binnacle.txt"
//Methods
void *initialMemory(MemoryLine *lines,int size);

int main() {
  int size;
  printf("Ingrese la cantidad de lineas de memoria: ");
  scanf("%d", &size);

  // Generate unique key to shmid
  key_t shmKey = ftok("shmfile", 21);
 
  // Identifier for shared memory
  int shmid = shmget(shmKey,size*sizeof(MemoryLine),0777|IPC_CREAT);

  // Pointer to shared memory
  MemoryLine *lines = (MemoryLine*) shmat(shmid,0,0);
  
  // Create binnacle file
  FILE* binnacle = fopen(file, "w");    

  //Initial memory
  initialMemory(lines,size);
  
return 0;
}
void *initialMemory(MemoryLine *lines,int size){
  for (int i = 0; i < size; i++)
  {
    lines[i].lineNumber = i+1;
    lines[i].available = true;
    lines[i].pid = NULL;
  }
}

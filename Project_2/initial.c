// run : gcc -o initial initial.c
#include <sys/file.h> // File managment
#include <sys/shm.h>  // Share memory with shmget
#include <sys/ipc.h>  // Generate ipc key to use with shmget
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/sem.h> // To implement semaphore
#include <stdlib.h>
#include "memory_line.h"
#include "shared_data.h"
#include "defines_values.h"

// Esta union hay que definirla o no según el valor de los defines aqui
// indicados.
//
#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
// La union ya está definida en sys/sem.h
#else
// Tenemos que definir la union
union semun
{
  int val;
  struct semid_ds *buf;
  unsigned short int *array;
  struct seminfo *__buf;
};
#endif

//Methods
void *initialMemory(MemoryLine *lines, int size);
struct sembuf operation;

int main()
{

  /****************************Create memory lines ****************************/
  int size;
  printf("Ingrese la cantidad de lineas de memoria: ");
  scanf("%d", &size);

  // Generate unique key to shmid
  key_t shmKey = ftok("/bin/ls", 20);

  // Identifier for shared memory
  int shmid = shmget(shmKey, size * sizeof(MemoryLine), 0777 | IPC_CREAT);

  // Pointer to shared memory
  MemoryLine *lines = (MemoryLine *)shmat(shmid, 0, 0);

  //Initial memory
  initialMemory(lines, size);

  //Update line values
  shmdt(lines);

  /****************************Create binnacle file***************************/
  FILE *binnacle = fopen(file, "w");
  char dWrite[100];

  sprintf(dWrite, "PID\t\t\t\t Tamaño\t\t\t\t Tipo\t\t\t\t Acción\t\t\t\t Hora\t\t\t\t Lineas\n");
  fputs(dWrite, binnacle);
  fclose(binnacle);

  /*****************************Create shared data****************************/

  // Generate unique key to shdid
  key_t shdKey = ftok("/bin/ls", 21);

  // Identifier for shared data
  int shdid = shmget(shdKey, 1 * sizeof(sharedData), 0777 | IPC_CREAT);

  // Pointer to shared data
  sharedData *data = (sharedData *)shmat(shdid, 0, 0);

  //Set memory size
  data->linesMemorySize = size;

  //Set threads size
  data->threadsSize = 0;

  //set false to finishFlg
  data->finishFlg = false;

  //Update data values
  shmdt(data);

  /*****************************Create shared threads**************************/

  // Generate unique key to shtid
  key_t shtKey = ftok("/bin/ls", 22);

  // Identifier for shared threads
  int shtid = shmget(shtKey, MAX_THREAD * sizeof(Thread), 0777 | IPC_CREAT | SHM_REMAP);

  /******************************Create semaphores ***************************/

  // Generate unique key to shsid
  key_t shsKey = ftok("/bin/ls", 23);

  // Identifier for semaphores
  // Create two semaphores 1 for shared memory and 2 for shared binnacle
  int shsid = semget(shsKey, 2, 0600 | IPC_CREAT);

  union semun arg;

  // Initial semaphore
  // 0 is red and 1 is green
  arg.val = 1;
  operation.sem_num = 0;
  operation.sem_op = 1;
  operation.sem_flg = 0;
  // 0 is semaphore for shared memory
  semctl(shsid, 0, SETVAL, &arg);
  semop(shsid, &operation, 1);
  
  // 1 is semaphore for ending status
  semctl(shsid, 1, SETVAL, &arg);
  operation.sem_num = 1;
  semop(shsid, &operation, 1);
  return 0;
}
void *initialMemory(MemoryLine *lines, int size)
{
  for (int i = 0; i < size; i++)
  {
    lines[i].lineNumber = i + 1;
    lines[i].available = true;
    lines[i].pid = (long unsigned int)NULL;
  }
}

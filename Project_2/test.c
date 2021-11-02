// run: gcc -o finisher finisher.c

#include <sys/file.h> // File managment
#include <sys/shm.h>  // Share memory with shmget
#include <sys/ipc.h>  // Generate ipc key to use with shmget
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

int main()
{

    /*****************************GET pointers to shared memory****************************/

    // shmget returnsan identifier in shdid (get sharedData memory id)
    int shdid = shmget(ftok("/bin/ls", 21), sizeof(sizeof(sharedData)), 0777);
    // shmat to attach to data shared
    data = (sharedData *)shmat(shdid, 0, 0);
    //semget returns an identifier in semKey (semaphores)
    semId = semget(ftok("/bin/ls", 23), 2, 0600);
    // shmget returns an identifier in shtid (get threads memory id)
    int shtid = shmget(ftok("/bin/ls", 22), data->threadsSize * sizeof(sizeof(Thread)), 0777);
    // shmat to attach to threads shared
    threads = (Thread *)shmat(shtid, 0, 0);
    // shmget returns an identifier in shmid (get MemoryLine memory id)
    int shmid = shmget(ftok("/bin/ls", 20), data->linesMemorySize * sizeof(MemoryLine), 0777);
    // shmat to attach to memoryLine Shared
    lines = (MemoryLine *)shmat(shmid, 0, 0);

    // send singal to finish to producer program
    // wait semaphore (decrese)
    /*operation.sem_num = 0;
    operation.sem_op = -1;
    operation.sem_flg = 0;
    semop(semId, &operation, 1);

    lines[0].available = false;
    lines[0].pid = 123456;
    lines[1].available = false;
    lines[1].pid = 123456;
    lines[2].available = false;
    lines[2].pid = 123456;
    lines[3].available = false;
    lines[3].pid = 123456;
    lines[5].available = false;
    lines[5].pid = 456789;
    lines[6].available = false;
    lines[6].pid = 456789;
    lines[7].available = false;
    lines[7].pid = 456789;
    lines[8].available = false;
    lines[8].pid = 456789;
    */
    /*data->threadsSize = 2;
    data->pidExecution = 987654;

    threads[0].alive = true;
    threads[0].blocked = false;
    threads[0].empty = false;
    threads[0].lines = 4;
    threads[0].pid = 123456;
    threads[0].time = 45;

    threads[1].alive = true;
    threads[1].blocked = true;
    threads[1].empty = false;
    threads[1].lines = 4;
    threads[1].pid = 456789;
    threads[1].time = 45;
    */

    // send singal to finish to producer program
    // wait semaphore (decrese)
    operation.sem_num = 0;
    operation.sem_op = -1;
    operation.sem_flg = 0;
    semop(semId, &operation, 1);
    // dettach
    operation.sem_num = 0;
    operation.sem_op = 1;
    operation.sem_flg = 0;
    semop(semId, &operation, 1);
    shmdt(lines);
    shmdt(data);
    shmdt(threads);

    // semaforos primero negativo y luego positivo (sem_op)
}

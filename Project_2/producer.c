// run gcc -o producer producer.c memory_line.c shared_data.c -lpthread

// libs
#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/file.h> // File managment


#include "memory_line.h"
#include "shared_data.h"
#include "defines_values.h"

// global variables
sharedData *data;
MemoryLine *lines;
Thread *threads;
int semId;
struct sembuf operation;
time_t ttime;
FILE *binnacle;

// function definition
bool bestFit(Thread *);
bool worstFit(Thread *);
bool firstFit(Thread *);
Thread *createNewThread();
void threadGenerate();
int getRandom(int lower, int upper);
int algorithmOption();
void threadFunction(void *args);
int memoryOption();

struct args
{
    int option;
    Thread *t;
};

bool bestFit(Thread *t)
{
    int index = 0;
    int bestBlockIndex = -1;
    MemoryBlock *blocks = getCurrentMemoryBlocks(lines, data->linesMemorySize);
    while (blocks[index].size != -1)
    {
        if (blocks[index].size >= t->lines)
        {
            if (bestBlockIndex != -1)
            {
                if (blocks[index].size < blocks[bestBlockIndex].size)
                    bestBlockIndex = index;
            }
            else
            {
                bestBlockIndex = index;
            }
        }
        index++;
    }
    //save thread lines in shared memory
    if (bestBlockIndex != -1)
    {
        saveThreadLines(lines, t->pid, t->lines, blocks[bestBlockIndex].startLine);
        char dWrite[150];
        char dateData[50];
        struct tm tm = *localtime(&ttime);
        sprintf(dateData, "now: %d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
        sprintf(dWrite, "%ld\t\t%d\t\t\t%s\t\t%s\t%s\t%d-%d\n", t->pid, t->lines, "Asignación", "Asignar espacio en memoria", dateData, blocks[bestBlockIndex].startLine, (blocks[bestBlockIndex].startLine + t->lines));
        fputs(dWrite, binnacle);
    }
    else
    {
        char dWrite[150];
        char dateData[50];
        struct tm tm = *localtime(&ttime);
        sprintf(dateData, "now: %d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
        sprintf(dWrite, "%ld\t\t%d\t\t\t%s\t\t\t%s\t\t%s\n", t->pid, t->lines, "Muerte", "Sin espacio en memoria", dateData);
        fputs(dWrite, binnacle);
    }
    // return true if thread found space in lines memory
    return bestBlockIndex != -1;
}

bool worstFit(Thread *t)
{
    int index = 0;
    int worstBlockIndex = -1;
    MemoryBlock *blocks = getCurrentMemoryBlocks(lines, data->linesMemorySize);
    while (blocks[index].size != -1)
    {
        if (blocks[index].size >= t->lines)
        {
            if (worstBlockIndex != -1)
            {
                if (blocks[index].size > blocks[worstBlockIndex].size)
                    worstBlockIndex = index;
            }
            else
            {
                worstBlockIndex = index;
            }
        }
        index++;
    }
    //save thread lines in shared memory
    if (worstBlockIndex != -1)
    {
        saveThreadLines(lines, t->pid, t->lines, blocks[worstBlockIndex].startLine);
        char dWrite[150];
        char dateData[50];
        struct tm tm = *localtime(&ttime);
        sprintf(dateData, "now: %d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
        sprintf(dWrite, "%ld\t\t%d\t\t\t%s\t\t%s\t%s\t%d-%d\n", t->pid, t->lines, "Asignación", "Asignar espacio en memoria", dateData, blocks[worstBlockIndex].startLine, (blocks[worstBlockIndex].startLine + t->lines));
        fputs(dWrite, binnacle);
    }
    else
    {
        char dWrite[150];
        char dateData[50];
        struct tm tm = *localtime(&ttime);
        sprintf(dateData, "now: %d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
        sprintf(dWrite, "%ld\t\t%d\t\t\t%s\t\t\t%s\t\t%s\n", t->pid, t->lines, "Muerte", "Sin espacio en memoria", dateData);
        fputs(dWrite, binnacle);
    }
    // return true if thread found space in lines memory
    return worstBlockIndex != -1;
}

bool firstFit(Thread *t)
{
    int index = 0;
    int found = 0;
    MemoryBlock *blocks = getCurrentMemoryBlocks(lines, data->linesMemorySize);
    while (blocks[index].size != -1)
    {
        if (blocks[index].size >= t->lines)
        {
            saveThreadLines(lines, t->pid, t->lines, blocks[index].startLine);
            found = 1;
            char dWrite[150];
            char dateData[50];
            struct tm tm = *localtime(&ttime);
            sprintf(dateData, "now: %d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
            sprintf(dWrite, "%ld\t\t%d\t\t\t%s\t\t%s\t%s\t%d-%d\n", t->pid, t->lines, "Asignación", "Asignar espacio en memoria", dateData, blocks[index].startLine, (blocks[index].startLine + t->lines));
            fputs(dWrite, binnacle);
            break;
        }
        index++;
    }

    if (found == 1)
    {
        return true;
    }
    else
    {
        char dWrite[150];
        char dateData[50];
        struct tm tm = *localtime(&ttime);
        sprintf(dateData, "now: %d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
        sprintf(dWrite, "%ld\t\t%d\t\t\t%s\t\t\t%s\t\t%s\n", t->pid, t->lines, "Muerte", "Sin espacio en memoria", dateData);
        fputs(dWrite, binnacle);
        return false;
    }
}

Thread *createNewThread()
{
    int lineSize = getRandom(LINE_RANGE);
    int executeTime = getRandom(THREAD_EXECUTE_TIME);
    int index = addThread(threads, data, lineSize, executeTime);
    return &threads[index];
}

// get a random number betwen lower and upper number
int getRandom(int lower, int upper)
{
    return (rand() % (upper - lower + 1)) + lower;
}

// function to create thread in range of 30-60 seconds
void threadGenerate()
{
    int option = memoryOption();
    // if thread found space in memory -> threadInMemory = true
    while (true)
    {
        //wait semaphore second semaphore
        operation.sem_num = 1;
        operation.sem_op = -1;
        operation.sem_flg = 0;
        semop(semId, &operation, 1);
        if (data->finishFlg)
        {
            // signal second semaphore
            operation.sem_num = 1;
            operation.sem_op = 1;
            operation.sem_flg = 0;
            semop(semId, &operation, 1);
            break;
        }
        // signal second semaphore
        operation.sem_num = 1;
        operation.sem_op = 1;
        operation.sem_flg = 0;
        semop(semId, &operation, 1);

        //wait
        operation.sem_num = 0;
        operation.sem_op = -1;
        operation.sem_flg = 0;
        semop(semId, &operation, 1);
        // if threads list is full, dont save thread in thread list
        if (MAX_THREAD >= data->threadsSize)
        {

            Thread *t = createNewThread();
            // signal
            operation.sem_num = 0;
            operation.sem_op = 1;
            operation.sem_flg = 0;
            semop(semId, &operation, 1);

            struct args *Args = (struct args *)malloc(sizeof(struct args));
            Args->option = option;
            Args->t = t;
            // created p_thread
            pthread_create(&(t->idPOSIX), NULL, (void *)threadFunction, (void *)Args);
        }
        else
        {
            //signal
            operation.sem_num = 0;
            operation.sem_op = 1;
            operation.sem_flg = 0;
            semop(semId, &operation, 1);
        }
        // sleep (30-60) seconds
        int rtime = getRandom(THREAD_TIME);
        printf("Program waiting %d seconds\n", rtime);
        sleep(rtime);
    }

    // wait for current threds exection
    printf("pthread_join...\n");
    for (int i = 0; i < MAX_THREAD; i++)
    {
        if (!threads[i].empty) {
            printf("Waiting for thread pid: %ld\n", threads[i].pid);
            void *retval;
            pthread_join(threads[i].idPOSIX, &retval); //to wait for thread in execution
        }
    }
}

void threadFunction(void *params)
{
    bool threadInMemory = false;
    Thread *t = ((struct args *)params)->t;
    t->pid =  syscall(SYS_gettid); // get thread id
    // run algorithm selected
    // wait semaphore
    operation.sem_num = 0;
    operation.sem_op = -1;
    operation.sem_flg = 0;
    t->blocked = true;
    semop(semId, &operation, 1);
    t->blocked = false;
    data->pidExecution = t->pid;
    switch (((struct args *)params)->option)
    {
    case 1: // First fit algorithm
        threadInMemory = firstFit(t);
        break;
    case 2: // Worst fit algorithm
        threadInMemory = worstFit(t);
        break;
    case 3: // Best fit algorithm
        threadInMemory = bestFit(t);
        break;
    }
     t->alive = threadInMemory;
    data->pidExecution = (pthread_t)-1;
    //signal sem
    operation.sem_num = 0;
    operation.sem_op = 1;
    operation.sem_flg = 0;
    semop(semId, &operation, 1);
    // time execution
    if (threadInMemory)
    {
        printf("Thread pid: %ld run in memory ,time: %d seconds, size: %d\n", t->pid, t->time, t->lines);
        sleep(t->time);
    } else{
        printf("Thread pid: %ld dont run in memory\n", t->pid);
    }
    // set free shared memory
    // wait semaphore
    operation.sem_num = 0;
    operation.sem_op = -1;
    operation.sem_flg = 0;
    semop(semId, &operation, 1);
    // thread leave the shared memory
    t->alive = false;
    // set free memory lines
    if (threadInMemory) {
        removeThreadLines(lines, t, data->linesMemorySize, binnacle);
    }
    removeThread(threads, data, t);
    //signal sem
    operation.sem_num = 0;
    operation.sem_op = 1;
    operation.sem_flg = 0;
    semop(semId, &operation, 1);
    pthread_exit(0);
}

int memoryOption()
{
    int option;
    printf("\t\tAllocationg memory algorithm option\n");
    printf("1. First fit algorithm\n");
    printf("2. Worst fit algorithm\n");
    printf("3. Best fit algorithm\n");
    printf("option: ");
    scanf("%d", &option);
    return option;
}

int main()
{
    // open binnacle
    binnacle = fopen(file, "a");
    // initiliaze time variable
    ttime = time(NULL);
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

    // call function generator
    threadGenerate();

    // close binnacle file
    fclose(binnacle);
    //detach from shared memory
    shmdt(lines);
    shmdt(data);
    shmdt(threads);

    // send signal to indicate that the producer has finished
    operation.sem_num = 1;
    operation.sem_op = 1;
    operation.sem_flg = 0;
    semop(semId, &operation, 1);
    printf("producer program has finished\n");
    return 0;
}
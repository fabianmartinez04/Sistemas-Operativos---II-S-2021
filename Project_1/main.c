// run using phtread library
// $ gcc -o main main.c matrix.c util.c -lpthread

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h> // thread library
#include <time.h>
#include <sys/mman.h> // shared memory
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/file.h>


#include "matrix.h"
#include "util.h"

// thread functions
void checkDirection(int row, int column, int direction);
void *runThread(void *t);
void threadsInit();
void threads_join();
void exitCell(int row, int column, char *type, int id);

// forks functions
void ForksInit();
void runForks();
void checkDirectionFork(int row, int column, int direction);

// write fork steps
void writeForkStep(Fork *fork);

// global variables

//start labyrinth
Labyrinth *game;
// List of thread
Thread *threads = NULL;
// List of Forks
Fork *forks = NULL;

// mutex init
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
// numbers of rows and columns of the matrix
int rowsN, columnsN;

// direction that it can take a thread or fork
enum
{
    UP = 0,
    DOWN = 1,
    LEFT = 2,
    RIGHT = 3
};

void writeForkStep(Fork *fork) {
    int status = -1;
    char data[20];
    while (status != 0)
    {
        FILE* f = fopen("steps.txt", "a");
        int fd = fileno(f);
        // flock should return zero on success
        status = flock(fd, LOCK_EX);
        if (status == 0) {
            pthread_mutex_lock(&lock);
            sprintf(data, "%d %d ", fork->pid, fork->direction);
            fputs(data, f);
            Step *iter = fork->steps;
            while (iter != NULL)
            {
                char c[10];
                sprintf(data, "%d %d ", iter->row, iter->column);
                fputs(data, f);
                iter = iter->next;
            }
            
            pthread_mutex_unlock(&lock);
            fputs("\n", f);
            flock(fd, LOCK_EX);
            fclose(f);
            break;
        } else {
            fclose(f);
        }    
    }
    
}

// look up for available direction to create new thread
void checkDirection(int row, int column, int direction)
{
    // check above
    if (direction != UP && row != 0 && direction != DOWN)
    {
        if (!game->cell[(row - 1) * rowsN + column].up && 
        !game->cell[row * rowsN + column].up &&
        !game->cell[(row - 1) * rowsN + column].isWall)
        {
            // create a new thread (row,colum, UP)
            Thread *t = createNewThread(row,column, UP);
            pthread_mutex_lock(&lock);
            game->cell[row * rowsN + column].up = 1;
            // unlock the shared resource
            pthread_mutex_unlock(&lock);
            addThreadAtEnd(threads, t);
            pthread_create((t->currentThread), NULL, runThread, t);

        }
    }
    // check below
    if (direction != DOWN && row != rowsN - 1 && direction != UP)
    {
        if (!game->cell[(row + 1) * rowsN + column].down && 
        !game->cell[row* rowsN + column].down && 
        !game->cell[(row + 1) * rowsN + column].isWall)
        {
            // create a new thread (row,colum, DOWN)
            Thread *t = createNewThread(row,column,DOWN);
            // lock the shared resource
            pthread_mutex_lock(&lock);
            game->cell[row * rowsN + column].down = 1;
            // unlock the shared resource
            pthread_mutex_unlock(&lock);
            addThreadAtEnd(threads, t);
            pthread_create((t->currentThread), NULL, runThread, t);
        }
    }
    // check left
    if (direction != LEFT && column != 0 && direction != RIGHT)
    {
        if (!game->cell[row*rowsN + column - 1].left && 
        !game->cell[row*rowsN + column].left && 
        !game->cell[row*rowsN + column - 1].isWall)
        {
            // create a new thread (row,colum, LEFT)
            Thread *t = createNewThread(row,column, LEFT);
            // lock the shared resource
            pthread_mutex_lock(&lock);
            game->cell[row * rowsN + column].left = 1;
            // unlock the shared resource
            pthread_mutex_unlock(&lock);
            addThreadAtEnd(threads, t);
            pthread_create((t->currentThread), NULL, runThread, t);   
        }
    }
    // check right
    if (direction != RIGHT && column != columnsN - 1 && direction != LEFT)
    {
        if (!game->cell[row*rowsN + column + 1].right && 
        !game->cell[row*rowsN + column].right &&
         !game->cell[row*rowsN + column + 1].isWall)
        {
            // create a new thread (row,colum, RIGHT)
            Thread *t = createNewThread(row,column, RIGHT);
            // lock the shared resource
            pthread_mutex_lock(&lock);
            game->cell[row * rowsN + column].right = 1;
            // unlock the shared resource
            pthread_mutex_unlock(&lock);
            addThreadAtEnd(threads, t);
            pthread_create((t->currentThread), NULL, runThread, t);
        }
    }
}

// join all threads
void threads_join() {
    Thread *aux = threads;
    while (aux != NULL)
    {
        pthread_join(*(aux->currentThread), NULL);
        aux = aux->next;
    };
};

// create first thread
void threadsInit(){
    // check if the right cell is available
    if (!game->cell[1].isWall) {
        // create a new thread (0,0, RIGHT);
        Step *s = malloc(sizeof(Step));
        s->row = 0;
        s->column = 0;
        s->next = NULL;
        threads->steps = s;
        threads->currentThread = (pthread_t *)malloc(sizeof(pthread_t));
        threads->next = NULL;
        threads->direction = RIGHT;
        pthread_create((threads->currentThread), NULL, runThread, threads);
    }
    // check if the down cell is available
    if (!game->cell[1*rowsN].isWall) {
        // create a new thread (0,0, DOWN);
        Thread *newItem = malloc(sizeof(pthread_t));
        Step *s = malloc(sizeof(Step));
        s->row = 0;
        s->column = 0;
        s->next = NULL;
        newItem->steps = s;
        newItem->currentThread = (pthread_t *)malloc(sizeof(pthread_t));
        newItem->next = NULL;
        newItem->direction = DOWN;
        // if a list of thread have element
        if (threads == NULL) {
            threads = newItem;
        } else { // thread list is empty
            addThreadAtEnd(threads,newItem);
        }
        pthread_create(newItem->currentThread, NULL, runThread, (void*) newItem);
    }
}

// thread initialization
void *runThread(void *t)
{
    Thread *thread = (Thread*)t;
    bool exit = 0;
    // move
    while (!exit)
    {
        Step *lastStep = getLastStepThread(thread);
        Step *next = malloc(sizeof(Step));
        // initialize values of the new step
        *next = (Step){
            .row = lastStep->row,
            .column = lastStep->column,
            .next = NULL
        };
        // lock the shared resource
        pthread_mutex_lock(&lock);
        if (game->cell[lastStep->row * rowsN + lastStep->column].exit)
        {
            exit = 1;
            // lock the shared resource
            pthread_mutex_unlock(&lock);
        }
        else
        {
            switch (thread->direction)
            {
            case UP:
                // check if a cell is in the first row
                if (lastStep->row == 0)
                {
                    exit = 1;
                }
                // get cell up of the last step: (row-1)*rowsN + column
                else if (!game->cell[(lastStep->row - 1) * rowsN + lastStep->column].up && !game->cell[(lastStep->row - 1) * rowsN + lastStep->column].isWall)
                {
                    game->cell[(lastStep->row - 1) * rowsN + lastStep->column].up = 1;
                    next->row = lastStep->row - 1;
                    lastStep->next = next;
                    
                }
                else
                {
                    exit = 1;
                }
                break;
            case DOWN:
                // check if a cell is in the last row
                if (lastStep->row == rowsN - 1)
                {
                    exit = 1;
                }
                // get cell down of the last step: (row+1)*rowsN + column
                else if (!game->cell[(lastStep->row + 1) * rowsN + lastStep->column].down && !game->cell[(lastStep->row + 1) * rowsN + lastStep->column].isWall)
                {
                    game->cell[(lastStep->row + 1) * rowsN + lastStep->column].down = 1;
                    next->row = lastStep->row + 1;
                    lastStep->next = next;
                }
                else
                {
                    exit = 1;
                }
                break;
            case LEFT:
                // check if a cell is in the first column
                if (lastStep->column == 0)
                {
                    exit = 1;
                }
                // get cell left of the last step: (row)*rowsN + column - 1
                else if (!game->cell[lastStep->row * rowsN + lastStep->column - 1].left && !game->cell[lastStep->row * rowsN + lastStep->column - 1].isWall)
                {
                    game->cell[lastStep->row * rowsN + lastStep->column - 1].left = 1;
                    next->column = lastStep->column - 1;
                    lastStep->next = next;
                }
                else
                {
                    exit = 1;
                }
                break;
            case RIGHT:
                // check if a cell is in the first column
                if (lastStep->column == columnsN - 1)
                {
                    exit = 1;
                }
                // get cell right of the last step: (row)*rowsN + column + 1
                else if (!game->cell[lastStep->row * rowsN + lastStep->column + 1].right && !game->cell[lastStep->row * rowsN + lastStep->column + 1].isWall)
                {
                    game->cell[lastStep->row * rowsN + lastStep->column + 1].right = 1;
                    next->column = lastStep->column + 1;
                    lastStep->next = next;
                }
                else
                {
                    exit = 1;
                }
                break;
            }
            exitCell(next->row,next->column, "Thread", 0);
            // lock the shared resource
            pthread_mutex_unlock(&lock);
            checkDirection(next->row,next->column,thread->direction);
        }
        if (exit == 1)
        {
            printf("Thread : %d finish\n", 0);
            // set free memory of the last sept
            free(next);
        }
    }
}

void exitCell(int row, int column, char *type, int id) {
    if (game->cell[row * rowsN + column].exit) {

        printf("%s : %d found the exit, row: %d, column: %d\n", type, id, row, column);
    }
}

// FORKS FUNCTIONS 

// create first forks
void ForksInit(){
    int status = 0;
    pid_t wpid;
    // check if the right cell is available
    if (!game->cell[1].isWall) {
        // create a new fork (0,0, RIGHT);
        forks = mmap(NULL, sizeof(Step), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        Step *s = mmap(NULL, sizeof(Step), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        s->row = 0;
        s->column = 0;
        s->next = NULL;
        forks->steps = s;
        forks->pid = getpid(), 
        forks->next = NULL;
        forks->direction = RIGHT;
    }
    // check if the down cell is available
    if (!game->cell[1*rowsN].isWall) {
        // create a new fork (0,0, DOWN);
        Fork *newItem = mmap(NULL, sizeof(Step), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        Step *s = mmap(NULL, sizeof(Step), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        s->row = 0;
        s->column = 0;
        s->next = NULL;
        newItem->steps = s;
        newItem->pid = getpid(),
        newItem->next = NULL;
        newItem->direction = DOWN;
        // if a list of thread is no empty
        if (forks == NULL) {
            forks = newItem;
            pid_t c_pid = fork();
            if (c_pid == 0) {
                forks->pid = getpid();
                runForks();
                exit(0);
            }
        } else { // forks list is no empty
            pid_t c_pid = fork();
            if (c_pid == 0) {
                newItem->pid = getpid();
                addForkAtEnd(forks,newItem);
                runForks();
                exit(0);
            }
        }
        while ((wpid = wait(&status)) > 0);
    }
}

// fork functions to evalute the labyrith
void runForks()
{
    Fork *f = getFork(forks, getpid());
    bool end = 0;
    // move
    while (!end)
    {
        Step *lastStep = getLastStepFork(f);
        Step *next = mmap(NULL, sizeof(Step), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        // initialize values of the new step
        *next = (Step){
            .row = lastStep->row,
            .column = lastStep->column,
            .next = NULL
        };
        // lock the shared resource
        pthread_mutex_lock(&lock);
        if (game->cell[lastStep->row * rowsN + lastStep->column].exit)
        {
            end = 1;
            // lock the shared resource
            pthread_mutex_unlock(&lock);
        }
        else
        {
            switch (f->direction)
            {
            case UP:
                // check if a cell is in the first row
                if (lastStep->row == 0)
                {
                    end = 1;
                }
                // get cell up of the last step: (row-1)*rowsN + column
                else if (!game->cell[(lastStep->row - 1) * rowsN + lastStep->column].up && !game->cell[(lastStep->row - 1) * rowsN + lastStep->column].isWall)
                {
                    game->cell[(lastStep->row - 1) * rowsN + lastStep->column].up = 1;
                    next->row = lastStep->row - 1;
                    lastStep->next = next;
                    
                }
                else
                {
                    end = 1;
                }
                break;
            case DOWN:
                // check if a cell is in the last row
                if (lastStep->row == rowsN - 1)
                {
                    end = 1;
                }
                // get cell down of the last step: (row+1)*rowsN + column
                else if (!game->cell[(lastStep->row + 1) * rowsN + lastStep->column].down && !game->cell[(lastStep->row + 1) * rowsN + lastStep->column].isWall)
                {
                    game->cell[(lastStep->row + 1) * rowsN + lastStep->column].down = 1;
                    next->row = lastStep->row + 1;
                    lastStep->next = next;
                }
                else
                {
                    end = 1;
                }
                break;
            case LEFT:
                // check if a cell is in the first column
                if (lastStep->column == 0)
                {
                    end = 1;
                }
                // get cell left of the last step: (row)*rowsN + column - 1
                else if (!game->cell[lastStep->row * rowsN + lastStep->column - 1].left && !game->cell[lastStep->row * rowsN + lastStep->column - 1].isWall)
                {
                    game->cell[lastStep->row * rowsN + lastStep->column - 1].left = 1;
                    next->column = lastStep->column - 1;
                    lastStep->next = next;
                }
                else
                {
                    end = 1;
                }
                break;
            case RIGHT:
                // check if a cell is in the first column
                if (lastStep->column == columnsN - 1)
                {
                    end = 1;
                }
                // get cell right of the last step: (row)*rowsN + column + 1
                else if (!game->cell[lastStep->row * rowsN + lastStep->column + 1].right && !game->cell[lastStep->row * rowsN + lastStep->column + 1].isWall)
                {
                    game->cell[lastStep->row * rowsN + lastStep->column + 1].right = 1;
                    next->column = lastStep->column + 1;
                    lastStep->next = next;
                }
                else
                {
                    end = 1;
                }
                break;
            }
            //printf("Fork: %d -> (%d,%d)\n", getpid(), next->row, next->column);    
            exitCell(next->row,next->column, "Fork", getpid());
            // unlock the shared resource
            pthread_mutex_unlock(&lock);
            checkDirectionFork(next->row,next->column,f->direction);
        }
        if (end == 1)
        {
            if (getpid != 0) {
                writeForkStep(f);
            }
            printf("Fork : %d finish\n", getpid());
            // set free memory of the last sept
            munmap(next, sizeof(Step));
        }
    }
}

// look up for available direction to create new fork
void checkDirectionFork(int row, int column, int direction)
{
    // check above
    if (direction != UP && row != 0 && direction != DOWN)
    {
        if (!game->cell[(row - 1) * rowsN + column].up && 
        !game->cell[row * rowsN + column].up &&
        !game->cell[(row - 1) * rowsN + column].isWall)
        {
            // create a new fork (row,colum, UP)
            Fork *f = createNewFork(row,column, UP);
            pthread_mutex_lock(&lock);
            game->cell[row * rowsN + column].up = 1;
            // unlock the shared resource
            pthread_mutex_unlock(&lock);
            addForkAtEnd(forks, f);
            // new fork
            pid_t pid = fork();
            if (pid == 0){
                f->pid = getpid();
                runForks();
            } else {
                // wait for child execute
                wait(NULL);
            }
        }
    }
    // check below
    if (direction != DOWN && row != rowsN - 1 && direction != UP)
    {
        if (!game->cell[(row + 1) * rowsN + column].down && 
        !game->cell[row* rowsN + column].down && 
        !game->cell[(row + 1) * rowsN + column].isWall)
        {
            // create a new thread (row,colum, DOWN)
            Fork *f = createNewFork(row,column, DOWN);
            // lock the shared resource
            pthread_mutex_lock(&lock);
            game->cell[row * rowsN + column].down = 1;
            // unlock the shared resource
            pthread_mutex_unlock(&lock);
            addForkAtEnd(forks, f);
            // new fork
            pid_t pid = fork();
            if (pid == 0){
                f->pid = getpid();
                runForks();
            }
            else {
                // wait for child execute
                wait(NULL);
            }
        }
    }
    // check left
    if (direction != LEFT && column != 0 && direction != RIGHT)
    {
        if (!game->cell[row*rowsN + column - 1].left && 
        !game->cell[row*rowsN + column].left && 
        !game->cell[row*rowsN + column - 1].isWall)
        {
            // create a new thread (row,colum, LEFT)
            Fork *f = createNewFork(row,column, LEFT);
            // lock the shared resource
            pthread_mutex_lock(&lock);
            game->cell[row * rowsN + column].left = 1;
            // unlock the shared resource
            pthread_mutex_unlock(&lock);
            addForkAtEnd(forks, f);
            // new fork
            pid_t pid = fork();
            if (pid == 0){
                f->pid = getpid();
                runForks();
            } else {
                // wait for child execute
                wait(NULL);
            }
        }
    }
    // check right
    if (direction != RIGHT && column != columnsN - 1 && direction != LEFT)
    {
        if (!game->cell[row*rowsN + column + 1].right && 
        !game->cell[row*rowsN + column].right &&
         !game->cell[row*rowsN + column + 1].isWall)
        {
            // create a new thread (row,colum, RIGHT)
            Fork *f = createNewFork(row,column, RIGHT);
            // lock the shared resource
            pthread_mutex_lock(&lock);
            game->cell[row * rowsN + column].right = 1;
            // unlock the shared resource
            pthread_mutex_unlock(&lock);
            addForkAtEnd(forks, f);
            // new fork
            pid_t pid = fork();
            if (pid == 0){
                f->pid = getpid();
                runForks();
            } else {
                // wait for child execute
                wait(NULL);
            }
        }
    }
}

int main()
{
    // set memory to game labyrith
    game = mmap(NULL, sizeof(Labyrinth), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    char filename[20] = "lab1.txt";
    //Read File
    FILE *filePointer;
    char firstLine[100];
    char *numbers;
    // to store the execution time of code
    double threadTime = 0.0;
    double forkTime = 0.0;
    // clocks
    clock_t begin;
    clock_t end;
    
    /*
    printf("File name: ");
    scanf("%s", filename);
    */
    
    //Name of the file
    filePointer = fopen(filename, "r");

    //validate if the file exists
    if (filePointer == NULL)
    {
        printf("File is not available \n");
    }
    else
    {
        // get columns and rows numbers
        fgets(firstLine, 100, filePointer);
        numbers = strtok(firstLine, " ");
        rowsN = getRowsNumber(numbers);
        columnsN = getColumnsNumber(numbers);
        // create a matrix in memory
        game->cell = labyrinth_init(rowsN, columnsN);

        // set values in each cell of the matrix
        readLabyrith(filePointer, game);
    }
    //close the file
    fclose(filePointer);

 

    // thread execution
    begin = clock();

    threadsInit();
    
    threads_join();

    end = clock();
 
    // calculate elapsed time by finding difference (end - begin) and
    // dividing the difference by CLOCKS_PER_SEC to convert to seconds
    threadTime += (double)(end - begin) / CLOCKS_PER_SEC;
 
    printf("\nThe elapsed time is %f seconds \n", threadTime);
    
    // forks execution
    // initialise mutex so it works properly in shared memory
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&lock, &attr);

    // set false all direction in each cell
    // rowsN * columnsN = total cell that were created
    setDefualtCellState(game, rowsN * columnsN);

    begin = clock();

    // method what execute forks
    ForksInit();

    end = clock();
    
    // calculate elapsed time by finding difference (end - begin) and
    // dividing the difference by CLOCKS_PER_SEC to convert to seconds
    forkTime += (double)(end - begin) / CLOCKS_PER_SEC;

    printf("\nThe elapsed time is %f seconds \n", forkTime);
    // segmentation sault - Sahred Memory created by child forks can't be access from mean fork
    /*if (forks->pid == getpid()) {
        
        Fork *iter = forks;
        while (iter != NULL) 
        {
            printf("Fork pid: %d, direction: %d\n", iter->pid, iter->direction);
            Step *auxStep = iter->steps;
            while (auxStep != NULL)
            {
                printf("(%d,%d)->", auxStep->row, auxStep->column);
                auxStep = auxStep->next;
            }
            printf("\n\n");
            iter = iter->next;        
        }
        printf("\n\n\n");
    }*/

    

    // set free matrix cell memory
    //munmap(game->cell, sizeof(game->cell));
    // set free threads
    //munmap(game, sizeof(game));

    Fork *forksList = readForkSteps();
    return 0;
}

// test
// print labyrith in console
/*for (int i = 0; i < rowsN * colum 
        }
    }*/
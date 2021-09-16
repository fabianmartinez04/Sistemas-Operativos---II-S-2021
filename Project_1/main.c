// run using phtread library
// $ gcc -o main main.c matrix.c util.c -lpthread

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h> // thread library

#include "matrix.h"
#include "util.h"

void checkDirection(int row, int column, int direction);
void *runThread(void *t);
void threadsInit();
void threads_join();

// global variables

//start labyrinth
Labyrinth game;
// List of thread
Thread *threads = NULL;
// mutex init
pthread_mutex_t lock;
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


// REVISAR PROBLEMA CON LA CREACIÓN DEL NUEVOS HILOS
// loop up for available direction to create new thread
void checkDirection(int row, int column, int direction)
{
    // check up
    if (direction != UP && row != 0)
    {
        if (!game.cell[(row - 1) * rowsN + column].up && !game.cell[(row - 1) * rowsN + column].isWall)
        {
            // create a new thread (row,colum, UP)
            Thread *t = createNewThread(row,column, UP);
            addElementAtEnd(threads, t);
            pthread_create((t->currentThread), NULL, runThread, t);
        }
    }
    if (direction != DOWN && row != rowsN - 1)
    {
        if (!game.cell[(row + 1) * rowsN + column].down && !game.cell[(row + 1) * rowsN + column].isWall)
        {
            // create a new thread (row,colum, DOWN)
            Thread *t = createNewThread(row,column,DOWN);
            addElementAtEnd(threads, t);
            pthread_create((t->currentThread), NULL, runThread, t);
        }
    }
    if (direction != LEFT && column != 0)
    {
        if (!game.cell[(row)*rowsN + column - 1].left && !game.cell[(row)*rowsN + column - 1].isWall)
        {
            // create a new thread (row,colum, LEFT)
            Thread *t = createNewThread(row,column, LEFT);
            addElementAtEnd(threads, t);
            pthread_create((t->currentThread), NULL, runThread, t);   
        }
    }
    if (direction != RIGHT && column != columnsN - 1)
    {
        if (!game.cell[(row)*rowsN + column + 1].right && !game.cell[(row)*rowsN + column + 1].isWall)
        {
            // create a new thread (row,colum, RIGHT)
            Thread *t = createNewThread(row,column, RIGHT);
            addElementAtEnd(threads, t);
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
    if (!game.cell[1].isWall) {
        // create a new thread (0,0, RIGHT);
        threads->steps->row = 0;
        threads->steps->column = 0;
        threads->steps->next = NULL;
        threads->currentThread = (pthread_t *)malloc(sizeof(pthread_t));
        threads->next = NULL;
        threads->direction = RIGHT;
        pthread_create((threads->currentThread), NULL, runThread, threads);
    }
    // check if the down cell is available
    if (!game.cell[1*rowsN].isWall) {
        // create a new thread (0,0, DOWN);
        Thread *newItem = malloc(sizeof(pthread_t));
        newItem->steps->row = 0;
        newItem->steps->column = 0;
        newItem->steps->next = NULL;
        newItem->currentThread = (pthread_t *)malloc(sizeof(pthread_t));
        newItem->next = NULL;
        newItem->direction = DOWN;
        // if a list of thread have element
        if (threads == NULL) {
            threads = newItem;
        } else { // thread list is empty
            addElementAtEnd(threads,newItem);
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
        Step *lastStep = getLastStep(thread);
        Step *next = malloc(sizeof(Step));
        // initialize values of the new step
        *next = (Step){
            .row = lastStep->row,
            .column = lastStep->column,
            .next = NULL
        };
        // lock the shared resource
        pthread_mutex_lock(&lock);
        if (game.cell[lastStep->row * rowsN + columnsN].exit)
        {
            exit = 1;
        }
        else
        {
            switch (thread->direction)
            {
            case UP:
                if (lastStep->row == 0)
                {
                    exit = 1;
                }
                // get cell up of the last step: (row-1)*rowsN + column
                else if (!game.cell[(lastStep->row - 1) * rowsN + lastStep->column].up || !game.cell[(lastStep->row - 1) * rowsN + lastStep->column].isWall)
                {
                    game.cell[(lastStep->row - 1) * rowsN + lastStep->column].up = 1;
                    next->row = lastStep->row - 1;
                    lastStep->next = next;
                }
                else
                {
                    exit = 1;
                }
                checkDirection(next->row,next->column,thread->direction);
                break;
            case DOWN:
                // get cell down of the last step: (row+1)*rowsN + column
                if (lastStep->row == columnsN - 1)
                {
                    exit = 1;
                }
                else if (!game.cell[(lastStep->row + 1) * rowsN + lastStep->column].down || !game.cell[(lastStep->row + 1) * rowsN + lastStep->column].isWall)
                {
                    game.cell[(lastStep->row + 1) * rowsN + lastStep->column].down = 1;
                    next->row = lastStep->row + 1;
                    lastStep->next = next;
                }
                else
                {
                    exit = 1;
                }
                checkDirection(next->row,next->column,thread->direction);
                break;
            case LEFT:
                // get cell left of the last step: (row)*rowsN + column - 1
                if (!game.cell[lastStep->row * rowsN + lastStep->column - 1].left || !game.cell[lastStep->row * rowsN + lastStep->column - 1].isWall)
                {
                    game.cell[lastStep->row * rowsN + lastStep->column - 1].left = 1;
                    next->column = lastStep->column - 1;
                    lastStep->next = next;
                }
                else
                {
                    exit = 1;
                }
                checkDirection(next->row,next->column,thread->direction);
                break;
            case RIGHT:
                // get cell right of the last step: (row)*rowsN + column + 1
                if (!game.cell[lastStep->row * rowsN + lastStep->column + 1].right || !game.cell[lastStep->row * rowsN + lastStep->column + 1].isWall)
                {
                    game.cell[lastStep->row * rowsN + lastStep->column + 1].right = 1;
                    next->column = lastStep->column + 1;
                    lastStep->next = next;
                }
                else
                {
                    exit = 1;
                }
                checkDirection(next->row,next->column,thread->direction);
                break;
            }
            // unlock shared resource
            pthread_mutex_unlock(&lock);
        }
        if (exit == 1)
        {
            // set free memory of the last sept
            free(next);
        }
    }
    pthread_exit(&game);
}

int main()
{

    char filename[20] = "lab1.txt";
    //Read File
    FILE *filePointer;
    char firstLine[100];
    char *numbers;

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
        game.cell = labyrinth_init(rowsN, columnsN);

        // set values in each cell of the matrix
        readLabyrith(filePointer, game);
    }
    //close the file
    fclose(filePointer);

    threadsInit();

    threads_join();

    // set free matrix cell memory
    free(game.cell);
    return 0;
}

// test
// print labyrith in console
/*for (int i = 0; i < rowsN * columnsN; i++)
    {   
        printf("%d ", game.cell[i].isWall);
        if ((i + 1) % columnsN == 0)
        {
            printf("\n");
        }
    }*/
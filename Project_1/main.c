// run using phtread library
// $ gcc -o main main.c matrix.c util.c -lpthread

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h> // thread library

#include "matrix.h"
#include "util.h"

// global variables

//start labyrinth
Labyrinth game;
// numbers of rows and columns of the matrix
int rowsN, columnsN;
// List of thread
Thread *threads;

// direction that it can take a thread or fork
enum
{
    UP = 0,
    DOWN = 1,
    LEFT = 2,
    RIGHT = 3
};

// thread initialization
void runThread(int direction, Thread *thread)
{
    bool exit = 0;
    // move
    while (!exit)
    {
        Step *lastStep = getLastStep(thread);
        Step *next = malloc(sizeof(Step));
        next->column = lastStep->column;
        next->row = lastStep->row;
        if (game.cell[lastStep->row * rowsN + columnsN].exit)
        {
            exit = 1;
        }
        else
        {
            switch (direction)
            {
            case UP:
                if (lastStep->row == 0)
                {
                    exit = 1;
                }
                // get cell up of the last step: (row-1)*rowsN + column
                /*semaphore implementation*/
                // mutex.h
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
                checkDirection(next->row,next->column,direction);
                break;
            case DOWN:
                // get cell down of the last step: (row+1)*rowsN + column
                /*semaphore implementation*/
                if (lastStep->row == columnsN - 1)
                {
                    exit = 1;
                }
                else if (!game.cell[(lastStep->row + 1) * rowsN + lastStep->column].down || !game.cell[(lastStep->row + 1) * rowsN + lastStep->column].isWall)
                {
                    game.cell[(lastStep->row + 1) * rowsN + lastStep->column].up = 1;
                    next->row = lastStep->row + 1;
                    lastStep->next = next;
                }
                else
                {
                    exit = 1;
                }
                checkDirection(next->row,next->column,direction);
                break;
            case LEFT:
                /*semaphore implementation*/
                // get cell left of the last step: (row)*rowsN + column - 1
                if (!game.cell[lastStep->row * rowsN + lastStep->column - 1].left || !game.cell[lastStep->row * rowsN + lastStep->column - 1].isWall)
                {
                    next->column = lastStep->column - 1;
                    lastStep->next = next;
                }
                else
                {
                    exit = 1;
                }
                checkDirection(next->row,next->column,direction);
                break;
            case RIGHT:
                /*semaphore implementation*/
                // get cell right of the last step: (row)*rowsN + column + 1
                if (!game.cell[lastStep->row * rowsN + lastStep->column + 1].right || !game.cell[lastStep->row * rowsN + lastStep->column + 1].isWall)
                {
                    next->column = lastStep->column + 1;
                    lastStep->next = next;
                }
                else
                {
                    exit = 1;
                }
                checkDirection(next->row,next->column,direction);
                break;
            }
        }
        if (exit == 1)
        {
            // set free memory of the last sept
            free(next);
        }
    }
};

// create first thread
void threadsInit(/*firstThread*/){
    // check if the right cell is available
    if (!game.cell[1].isWall) {
        // create a new thread (0,0, RIGHT);
    }
    // check if the down cell is available
    if (!game.cell[1*rowsN].isWall) {
        // create a new thread (0,0, DOWN);
    }
};

// loop up for available direction to create new thread
void checkDirection(int row, int column, int direction)
{
    // check up
    if (direction != UP && row != 0)
    {
        if (!game.cell[(row - 1) * rowsN + column].up || !game.cell[(row - 1) * rowsN + column].isWall)
        {
            // create a new thread (row,colum, UP)
        }
    }
    if (direction != DOWN && row != rowsN - 1)
    {
        if (!game.cell[(row + 1) * rowsN + column].down || !game.cell[(row + 1) * rowsN + column].isWall)
        {
            // create a new thread (row,colum, DOWN)
        }
    }
    if (direction != LEFT && column != 0)
    {
        if (!game.cell[(row)*rowsN + column - 1].left || !game.cell[(row)*rowsN + column - 1].isWall)
        {
            // create a new thread (row,colum, LEFT)
        }
    }
    if (direction != RIGHT && column != columnsN - 1)
    {
        if (!game.cell[(row)*rowsN + column + 1].right || !game.cell[(row)*rowsN + column + 1].isWall)
        {
            // create a new thread (row,colum, RIGHT)
        }
    }
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

    for (int i = 0; i < rowsN * columnsN; i++)
    {
        printf("%d ", game.cell[i].isWall);
        if ((i + 1) % columnsN == 0)
        {
            printf("\n");
        }
    }

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
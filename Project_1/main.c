// run using phtread library
// $ gcc -o example example.c -lpthread

#include <stdio.h>
#include <string.h>
#include <pthread.h>  // thread library

#include "matrix.h"
#include "util.h"


int main() 
{
    char filename[20];
    //Read File
    FILE *filePointer;
    char firstLine[100];
    int rowsN, columnsN;
    char *numbers;

    //start labyrinth
    Labyrinth game;
    
    printf("File name: ");
    scanf("%s", filename);

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
        fgets (firstLine, 100, filePointer);
        numbers = strtok(firstLine, " ");
        rowsN = getRowsNumber(numbers);
        columnsN = getColumnsNumber(numbers);
        // create a matrix in memory
        game.cell = labyrinth_init(rowsN,columnsN);

        // set values in each cell of the matrix
        readLabyrith(filePointer,game);

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
}


    // test
    // print labyrith in console
    /*for (int i = 0; i < rowsN * columnsN; i++)
    {   
        printf("%d ", game.cell[i].isWall);
        if ((i + 1) % c == 0)
        {
            printf("\n");
        }
    }*/
// run using phtread library
// $ gcc -o example example.c -lpthread

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

// thread library
#include <pthread.h>

/*CONSTANTS*/


// struct: cell
// this struct represent a cell of the matrix
typedef struct Cell
{
    // indicate if a thread/fork has already passed throught the cell in that direction
    bool left, right, up, down;
    bool exit; // Flag indicate exit cell of the labyrinth
    bool isWall;
} Cell;


// struct: Labyrinth
// represent a matrix of cell
typedef struct  Labyrinth{
    Cell *cell;
} Labyrinth;


// struct: Step
// save each step of the thread or fork in the labyrith
typedef struct Step
{
    int row;
    int column;
    struct Step *next;
}Step;


// struct: thread
// represent each thread in the matrix
typedef struct Thread{
    int id;
    Step *first;
} Thread;


//Dynamically allocating memory
Cell *labyrinth_init(int rows, int column) {
    return calloc(rows * column, sizeof(Cell)); 
}


void readLabyrith(FILE *filePointer, Labyrinth game) {
    char ch;
    int i = 0;
    //get character by character from file
    while ((ch = fgetc(filePointer)) != EOF)
    {
        //decide is if wall or exit
        if (ch == '*'){
            game.cell[i].isWall = 1;
        }
        //exit cell
        else if(ch == '/'){
            game.cell[i].exit = 1;
        }
        //validate just this chars
        if (ch == '*' ||  ch == '/' ||  ch == ' ') { 
            i++;
        }
    }
}

// get numbers number from file
int getRowsNumber(char *numbers) {
    if(numbers){
        return atoi(numbers); //Taking number of Rows
    }
    return 0;
}

//column number from file
int getColumnsNumber(char *numbers) {
    // remove first number that refers to rows columns
    numbers = strtok(NULL, " ");

    if(numbers) {
        return atoi(numbers); //Taking number of cols
    }
    return 0;
}


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
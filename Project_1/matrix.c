#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

/*CONSTANTS*/


// struct: cell
// this struct represent a cell of the matrix

typedef struct
{
    // indicate if a thread/fork has already passed throught the cell in that direction
    bool left, right, up, down;
    bool exit; // Flag indicate exit cell of the labyrinth
    bool isWall;
} Cell;


// struct: Labyrinth
// represent a matrix of cell
typedef struct  {
    Cell *cell;
} Labyrinth;

//Dynamically allocating memory
Cell *labyrinth_init(int rows, int column) {
    return calloc(rows * column, sizeof(Cell)); 
}

int main() 
{

    //Read File
    FILE *filePointer;
    char ch;
    char firstLine[100];
    char *numbers;
    int r, c;

    //Name of the file
    filePointer = fopen("lab1.txt", "r");

    //validate if the file exists 
    if (filePointer == NULL)
    {
        printf("File is not available \n");
    }
    else
    {

        fgets (firstLine, 100, filePointer);
        numbers = strtok(firstLine, " ");
        
        if(numbers){
            //printf("Fila%s\n", rows);
            r = atoi(numbers); //Taking number of Rows
        }
        numbers = strtok(NULL, " ");

        if(numbers)
            //printf("Columnas%s\n", rows);
            c = atoi(numbers); //Taking number of cols
    
        //get character by character from file
        while ((ch = fgetc(filePointer)) != EOF)
        {
            printf("%c\n", ch);
        }
    }

    //close the file
    fclose(filePointer);

    
    Labyrinth game;
    
    game.cell = labyrinth_init(r,c);
    // test
    for (int i = 0; i < r * c; i++)
    {   
        if (i % 2 == 0) {
            game.cell[i].isWall = 1;
        } else {
            game.cell[i].isWall = 0;
        }
        printf("%d ", game.cell[i].isWall);
        if ((i + 1) % c == 0)
        {
            printf("\n");
        }
    }
    free(game.cell);
}
#ifndef MATRIX_H
#define MATRIX_H

// includes
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


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
typedef struct{
    Cell *cell;
} Labyrinth;


//create dynamically allocating memory for each cell
Cell *labyrinth_init(int rows, int columns);

// set each wall and exit in the matrix
void readLabyrith(FILE *filePointer, Labyrinth *game);

// get numbers number from file
int getRowsNumber(char *numbers);

//column number from file
int getColumnsNumber(char *numbers);

// set default state in each cell direction 
void setDefualtCellState(Labyrinth *game, int lenght);

#endif /*MATRIX_H*/
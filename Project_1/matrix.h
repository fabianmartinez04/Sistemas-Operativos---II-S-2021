#ifndef MATRIX_H
#define MATRI_H

// includes
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>


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
Cell *labyrinth_init(int rows, int olumns);


void readLabyrith(FILE *filePointer, Labyrinth game);

// get numbers number from file
int getRowsNumber(char *numbers);

//column number from file
int getColumnsNumber(char *numbers);

#endif /*MATRIX_H*/
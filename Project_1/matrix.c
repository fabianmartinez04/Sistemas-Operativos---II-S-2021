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
    Labyrinth game;
    
    int r = 4, c = 4; //Taking number of Rows and Columns
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
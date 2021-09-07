#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// struct: cell
// this struct represent a cell of the matrix

struct Wall{

} ;

struct Floor
{
    // indicate if a thread/fork has already passed throught the cell in that direction
    bool left, right, up, down;
    bool exit; // Flag indicate exit cell of the labyrinth
};


typedef struct Cell{
    union type{
        struct Floor floor;
        struct Wall Wall;
    } ;
} Cell;


// struct: matrix

struct matrix {
    
};
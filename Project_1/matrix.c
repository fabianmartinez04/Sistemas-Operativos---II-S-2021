#include "matrix.h"

//Dynamically allocating memory
Cell *labyrinth_init(int rows, int columns) {
    return calloc(rows * columns, sizeof(Cell)); 
};
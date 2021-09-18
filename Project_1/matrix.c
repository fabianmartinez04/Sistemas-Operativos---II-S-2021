#include "matrix.h"

#include <string.h>
#include <sys/mman.h> // shared memory

//Dynamically allocating memory
Cell *labyrinth_init(int rows, int columns) {
    return mmap(NULL, rows * columns * sizeof(Cell), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
};

// set each wall and exit from file
void readLabyrith(FILE *filePointer, Labyrinth *game) {
    char ch;
    int i = 0;
    //get character by character from file
    while ((ch = fgetc(filePointer)) != EOF)
    {
        //decide is if wall or exit
        if (ch == '*'){
            game->cell[i].isWall = 1;
        }
        //exit cell
        else if(ch == '/'){
            game->cell[i].exit = 1;
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

// set default state in each cell direction 
void setDefualtCellState(Labyrinth *game, int lenght) {
    for (int i = 0; i < lenght; i++)
    {
        game->cell[i].up = 0;
        game->cell[i].down = 0;
        game->cell[i].right = 0;
        game->cell[i].left = 0;
    }
}
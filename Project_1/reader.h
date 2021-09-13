#ifndef READER_H
#define READER_H

#include "matrix.h"
#include <stdio.h>
#include <string.h>

void readLabyrith(FILE *filePointer, Labyrinth game);

// get numbers number from file
int getRowsNumber(char *numbers);

//column number from file
int getColumnsNumber(char *numbers);

#endif /*READER_H*/
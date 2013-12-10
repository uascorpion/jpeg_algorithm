#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "types.h"


void print1dmas(byte_t * inputmas, int sizeX, int sizeY);
void print2dmas(float** mas, int sizeX, int sizeY);

float** transMatrix(float** inMas, int sizeX, int sizeY);
float** multMatrix(float** firstMas, float** secondMas, int sizeM);

#endif // MATRIX_H_INCLUDED

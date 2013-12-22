#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "types.h"

void print1dmas(float* inputmas, int elNum);
void print1dImas(int* inputmas, int elNum);
void print1dCmas(char* inputmas, int elNum);
void print2dFmas(float** mas, int sizeX, int sizeY);
void print2dImas(int** mas, int sizeX, int sizeY);

float** transMatrix(float** inMas, int sizeX, int sizeY);
float** multMatrix(float** firstMas, float** secondMas, int sizeM);
float** divideMatrixByMatrix(float** firstMas, float** secondMas);

#endif // MATRIX_H_INCLUDED

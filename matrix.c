#include "matrix.h"

void print1dmas(byte_t * inputmas, int sizeX, int sizeY)
{
    int x, y;
    for(y = 0; y < sizeY; y++)
    {
        for (x = 0; x < sizeX; x++)
        {
            printf("%4d ", inputmas[x + y * sizeX]);
        }
        printf("\n");
    }
}

void print2dmas(float** mas, int sizeX, int sizeY)
{
    int x,y;
    for (y = 0; y < sizeY; y++) {
        for (x = 0; x < sizeX; x++) {
            printf("%3.2f ",mas[y][x]);
        }
        printf("\n");
    }
}

/* Receive matrix, transposed to current */
float** transMatrix(float** inMas, int sizeX, int sizeY)
{
    int i,j;
    float ** outMas = (float **)malloc(sizeY * sizeof(float *));
    for (i = 0; i < sizeX; i++ ) {
        outMas[i] = (float *)malloc(sizeX * sizeof(float));
    }
    for (i = 0; i < sizeY; i++) {
        for (j = 0; j < sizeX; j++) {
            outMas[i][j] = inMas[j][i];
        }
    }
    return outMas;
}

/* Multiplaying of two square float martix with size equal to sizeM */
float** multMatrix(float** firstMas, float** secondMas, int sizeM)
{
    float** resMas = (float**)malloc(sizeM * sizeof(float *));
    int i,j,k;
    float element;
    for (i = 0; i < sizeM; i++) {
        resMas[i] = (float*)malloc(sizeM * sizeof(float));
    }
    for (i = 0; i < sizeM; i++) {
        for (j = 0; j < sizeM; j++) {
            for (k = 0; k < sizeM; k++) {
                element += firstMas[i][k] * secondMas[k][j];
            }
            resMas[i][j] = element;
            element = 0;
        }
    }
    return resMas;
}

float** divideMatrixByMatrix(float** firstMas, float** secondMas)
{
    int i,j;
    float** resMas = (float**)malloc(8 * sizeof(float*));
    for (i = 0; i < 8; i++) {
        resMas[i] = (float*)malloc(8 * sizeof(float));
    }

    for(i = 0; i < 8; i++) {
        for(j = 0; j < 8; j++) {
            resMas[i][j] = roundf(firstMas[i][j] / secondMas[i][j]);
        }
    }

    return resMas;
}

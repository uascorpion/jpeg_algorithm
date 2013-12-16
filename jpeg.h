#ifndef JPEG_H_INCLUDED
#define JPEG_H_INCLUDED

#include <math.h>
#include "bmp.h"
#include "matrix.h"
#include "types.h"

#pragma pack(push, 1)

/* Structure dor default rgb-palette. Every byte is a color */
typedef struct {
    int Y;
    int Cb;
    int Cr;
} color_YCbCr;

#pragma pack(pop)

void print1RGB(palette_rgb* inMas, long long int elnum);
void print1YCbCr(color_YCbCr * inMas, long long int elnum);

int* orderZigZag(float** input_mas, byte_t sizeX, byte_t sizeY);

color_YCbCr convertRGBtoYCbCr(palette_rgb inPixel);

float** calculateDCTmatrix(void);
float** calcQuantMatrix(int quality);

color_YCbCr* divideImageBySquers(color_YCbCr* mas,int sizeX, int sizeY, int curNumOfSqrX, int curNumOfSqrY, int numOfSqrX);
void convertToJpeg (palette_rgb* inputMas, dword_t sizeX, dword_t sizeY, int quality);

#endif // JPEG_H_INCLUDED

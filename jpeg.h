#ifndef JPEG_H_INCLUDED
#define JPEG_H_INCLUDED

#include <math.h>
#include "bmp.h"
#include "matrix.h"
#include "types.h"
#include <stdbool.h>
#include <string.h>

int* orderZigZag(float** input_mas, byte_t sizeX, byte_t sizeY);

RLE_mas CodingRLE(int* inputMas);

RC_pair getRCpair(int inputAC);
char_mas Huffman(int Relem, int Zelem, bool isY);
char_mas codingDC(int inputDC);
char_mas codingAC(int inputAC,int inputZ, bool isY);

color_YCbCr convertRGBtoYCbCr(palette_rgb inPixel);

float** calcDCTmatrix(void);
float** calcQuantMatrix(int quality);

color_YCbCr* divideImageBySquers(color_YCbCr* mas, int sizeX, int sizeY, int curNumOfSqrX, int curNumOfSqrY, int numOfSqrX);

void convertToJpeg (palette_rgb* inputMas, dword_t sizeX, dword_t sizeY, int quality);

#endif // JPEG_H_INCLUDED

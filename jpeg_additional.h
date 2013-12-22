#ifndef JPEG_ADDITIONAL_H_INCLUDED
#define JPEG_ADDITIONAL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "types.h"

void print1RGB(palette_rgb* inMas, long long int elnum);
void print1YCbCr(color_YCbCr * inMas, long long int elnum);

char* dec2bin(int n, int codeLong);

#endif // JPEG_ADDITIONAL_H_INCLUDED

#ifndef JPEG_ADDITIONAL_H_INCLUDED
#define JPEG_ADDITIONAL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include <string.h>

void print1RGB(palette_rgb* inMas, long long int elnum);
void print1YCbCr(color_YCbCr * inMas, long long int elnum);

char* dec2bin(int n, int codeLong);
long long int bin2dec(char *bin);

#endif // JPEG_ADDITIONAL_H_INCLUDED

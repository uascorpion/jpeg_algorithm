#ifndef BMP_H_INCLUDED
#define BMP_H_INCLUDED

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "types.h"

#define FROM_START      0U
#define FROM_CUR_POS    1U
#define FROM_END        2U
#define PERMS           0666

typedef unsigned short int compression_t;
enum compression_t { BI_RGB, BI_RLE8, BI_RLE4, BI_BITFIELDS, BI_JPEG, BI_PNG, BI_ALPHABITFIELDS };

typedef unsigned short int orientation_t;
enum orientation_t { NORMAL, MIRROR };

volatile BMPfileHeader cur_file_header;
volatile BMPinfoHeader cur_info_header;

dword_t img_height;
dword_t img_width;
int img_bitCount;
compression_t img_compression;
orientation_t orientation;
long long int image_dots;

palette_rgb* bmp_parse(int fd);

#endif /* BMP_H_INCLUDED */

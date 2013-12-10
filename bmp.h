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

void bmp_parse(int fd);

#pragma pack(push, 1)

/* Definition of BMP-file header structure */
typedef struct {
    word_t  bfType;             /* Signature BM*/
    dword_t bfSize;             /* File size in bytes */
    word_t  bfReserved1;        /* Reserved 1*/
    word_t  bfReserved2;        /* Reserved 2 */
    dword_t bfOffBits;          /* File offset to pixel array */
} BMPfileHeader;

/* Definition of BMP image info header */
typedef struct {
    dword_t biSize;             /* Header info size */
    dword_t biWidth;            /* Image WIDTH (pixels ) */
    dword_t biHeight;           /* Image HEIGHT (pixels) */
    word_t  biPlanes;           /* Planes of image (standart value 1) */
    word_t  biBitCount;         /* Bit per pixel */
    dword_t biCompression;      /* Commpression type */
    dword_t biSizeImage;        /* 0 or Compressed Image Size */
    dword_t biXPelsPerMeter;    /* X Pixels per meter */
    dword_t biYPelsPerMeter;    /* Y pixels per meter */
    dword_t biClrUsed;          /* Number of used colors */
    dword_t biClrImportant;     /* Number of important colors */
} BMPinfoHeader;

/* Stucture of element of color palette for 8-bit images (with 256 colors) */
typedef struct {
    byte_t blue   : 8;
    byte_t green  : 8;
    byte_t red    : 8;
    byte_t unused : 8;
} palette_8_bit;

/* Structure dor default rgb-palette. Every byte is a color */
typedef struct {
    byte_t blue   : 8;
    byte_t green  : 8;
    byte_t red    : 8;
} palette_rgb;

/* Structure of elements of color palette for 16-bit images */
typedef struct {
    word_t blue_mask  : 16;
    word_t green_mask : 16;
    word_t red_mask   : 16;
} palette_16_bit;

/* Structures for 16-bit images with 2^15 colors */
typedef struct {
    byte_t blue   : 5;
    byte_t green  : 5;
    byte_t red    : 5;
    byte_t unused : 1;
} palette_rgb_5_5_5;

/* Structures for 16-bit images with 2^16 colors */
typedef struct {
    byte_t blue   : 5;
    byte_t green  : 6;
    byte_t red    : 5;
    byte_t unused : 1;
} palette_rgb_5_6_5;

/* Structure of masks for every color */
typedef struct {
    dword_t blue_mask;
    dword_t green_mask;
    dword_t red_mask;
} palette_biCompression;

#pragma pack(pop)

typedef unsigned short int compression_t;
enum compression_t { BI_RGB, BI_RLE8, BI_RLE4, BI_BITFIELDS, BI_JPEG, BI_PNG, BI_ALPHABITFIELDS };

typedef unsigned short int orientation_t;
enum orientation_t { NORMAL, MIRROR };

volatile BMPfileHeader cur_file_header;
volatile BMPinfoHeader cur_info_header;

int img_height;
int img_width;
int img_bitCount;
compression_t img_compression;
orientation_t orientation;
long long int image_dots;

#endif /* BMP_H_INCLUDED */

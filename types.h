#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED

typedef unsigned short int word_t;
typedef unsigned long int dword_t;
typedef unsigned char byte_t;

#define M_PI		3.14159265358979323846

#pragma pack(push, 1)

/*
    Special types for BMP
*/

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

/*
    Special types for JPEG
*/

/* Structure dor default rgb-palette. Every byte is a color */
typedef struct {
    int Y;
    int Cb;
    int Cr;
} color_YCbCr;

#pragma pack(pop)

typedef struct {
    int* mas;
    int  MasSize;
} RLE_mas;

typedef struct {
    char* mas;
    int   MasSize;
} char_mas;

typedef struct {
    int R;
    int C;
} RC_pair;

typedef struct {
    int R;
    int Z;
} RZ_pair;
#endif /* TYPES_H_INCLUDED */

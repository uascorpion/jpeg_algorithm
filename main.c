#include "bmp.h"
#include "jpeg.h"
#include "matrix.h"

extern volatile BMPfileHeader cur_file_header;
extern volatile BMPinfoHeader cur_info_header;

int main()
{
    int fd = open("C:\\Users\\Paul\\workspace\\jpeg_algorithm\\melexis.bmp",O_RDONLY,0);    /* Creationg file descriptor */

    if (-1 == fd) {
        printf("Can't open file\n");
    }

    else {
        palette_rgb* bmpimg = bmp_parse(fd);
        convertToJpeg(bmpimg, img_width, img_height, 3);
    }

/*
    float input_mas[8][8] = {{1,2,3,4,5,6,7,8},{9,10,11,12,13,14,15,16},{17,18,19,20,21,22,23,24},
                             {25,26,27,28,29,30,31,32},{33,34,35,36,37,38,39,40},{41,42,43,44,45,46,47,48},
                             {49,50,51,52,53,54,55,56},{57,58,59,60,61,62,63,64}};
*/
/*
    float** input_mas = (float**)malloc(8 * sizeof(float*));
    int i,j;
    for (i = 0; i < 8; i++) {
        input_mas[i] = (float*)malloc(8 * sizeof(float));
    }

    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            input_mas[i][j] = i * 8 + j + 1;
        }
    }

    print2dmas(input_mas, 8, 8);
    printf("\n");
    float* mas = orderZigZag(input_mas, 8, 8);

    byte_t x,y = 0;
    for (y = 0; y < 8; y++) {
        for (x = 0; x < 8; x++) {
            printf("%f ",mas[y*8 + x]);
        }
        printf("\n\n");
    }
*/
/*
    int * mas = malloc(512 * sizeof(int));
    int i,j;
    for (i=0; i < 512; i++)
    {
        mas[i] = i;
    }

    int sizeX = 32;
    int sizeY = 16;
*/
    //divideImageBySquers(mas, sizeX, sizeY);
/*
    print2dmas(transMatrix(calculateDCTmatrix(), 8, 8), 8, 8);

    float** mas1 = (float**)malloc(2 * sizeof(float*));
    for(i = 0;i < 2; i++) {
        mas1[i] = (float*)malloc(2 * sizeof(float));
    }

    float** mas2 = (float**)malloc(2 * sizeof(float*));
    for(i = 0; i < 2; i++) {
        mas2[i] = (float*)malloc(2 * sizeof(float));
    }

    for (i = 0; i < 2; i++) {
        for(j = 0; j < 2; j++) {
            mas1[i][j] = 1.0 + (float)(i * 2 + j);
        }
    }

    for (i = 0; i < 2; i++) {
        for(j = 0; j < 2; j++) {
            mas2[i][j] = 5.0 + (float)(i * 2 + j);
        }
    }

    printf("\n");
    print2dmas(mas1,2,2);
    printf("\n");
    print2dmas(mas2,2,2);
    printf("\n");
    print2dmas(multMatrix(mas1,mas2,2),2,2);
*/
    return 0;
}

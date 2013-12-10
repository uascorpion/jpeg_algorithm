#include "jpeg.h"

/* Zig-zag ordering elements of sizeX x sizeY array */
byte_t** orderZigZag (byte_t* input_mas, byte_t sizeX, byte_t sizeY)
{
/* printing input matix
    for (y = 0; y < sizeY; y++) {
        for (x = 0; x < sizeX; x++) {
            printf("%2d ",input_mas[x + y * sizeX]);
        }
        printf("\n\n");
    }
*/
    byte_t **output_mas = (byte_t **)malloc(sizeY * sizeof(byte_t *));
    int i = 0;
    for(i = 0; i < sizeY; i++) {
        output_mas[i] = (byte_t *)malloc(sizeX * sizeof(byte_t));
    }

    byte_t x,y,num;
    x = y = num = 0;
    output_mas[y][x] = input_mas[num];
    do {
        if (x < (sizeX-1)) {
            output_mas[y][++x] = input_mas[++num];
        }
        else {
            output_mas[++y][x] = input_mas[++num];
        }

        while (x > 0) {
            if (y == (sizeY-1)) break;
            output_mas[++y][--x] = input_mas[++num];
        };

        if (y < (sizeY-1)) {
            output_mas[++y][x] = input_mas[++num];
        }
        else {
            output_mas[y][++x] = input_mas[++num];
        }

        while (y > 0) {
            if (x == (sizeX-1)) break;
            output_mas[--y][++x] = input_mas[++num];
        };
    } while (num < (sizeX * sizeY - 1));

    return output_mas;
}

/* Converting color scheme from RGB to YCbCr */
color_YCbCr convertRGBtoYCbCr (palette_rgb inPixel) {
    color_YCbCr outPixel;
    outPixel.Y = 0.299 * inPixel.red + 0.587 * inPixel.green + 0.114 * inPixel.blue;
    outPixel.Cr = 128 + 0.5 * inPixel.red - 0.418688 * inPixel.green - 0.081312 * inPixel.blue;
    outPixel.Cb = 128 - 0.168736 * inPixel.red - 0.331264 * inPixel.green + 0.5 * inPixel.blue;
    return outPixel;
}

/* Dividing image matrix with size sizeX x sizeY in number of matrix with sizes 8 x 8  */
void divideImageBySquers(int * mas, int sizeX, int sizeY)
{
    int numOfSqrX = sizeX / 8;
    int numOfSqrY = sizeY / 8;

    int curNumOfSqrX, curNumOfSqrY;
    curNumOfSqrY = 0;
    curNumOfSqrX = 0;
    int i,j;

    byte_t * outMas = malloc(64 * sizeof(byte_t));

    while (curNumOfSqrY < numOfSqrY)
    {
        while (curNumOfSqrX < numOfSqrX)
        {
            printf("Matrix %d-%d\n", curNumOfSqrX, curNumOfSqrY);
            for(j = 0; j < 8; j++)
            {
                for (i = 0; i < 8; i++)
                {
                    outMas[i + j * 8] = mas[i + curNumOfSqrX * 8 + sizeX * j + curNumOfSqrY * 64 * numOfSqrX];
                }
            }
            print1dmas(outMas, 8, 8);
            printf("\n");
            //print2dmas(orderZigZag(outMas, 8, 8),8,8);
            curNumOfSqrX++;
            printf("\n\n");
        }
        curNumOfSqrX = 0;
        curNumOfSqrY++;
        printf("\n\n");
    }
}

/* Calculating of Discrete Cosine Transform matrix for size "8" */
float** calculateDCTmatrix(void)
{
    int i, j, sizeX, sizeY;
    float N;
    sizeX = sizeY = N = 8;
    float ** DCT = (float **)malloc(sizeY * sizeof(float *));
    for (i = 0; i < sizeX; i++ ) {
        DCT[i] = (float *)malloc(sizeX * sizeof(float));
    }

    for (i = 0; i < sizeY; i++) {
        for (j = 0; j < sizeX; j++) {
            if (i == 0) {
                DCT[i][j] = 1.0 / sqrt(N);
            }
            else {
                DCT[i][j] = sqrt(2.0 / N) * cosf( (2.0 * (float)j + 1.0) * (float)i * M_PI / 2.0 / N);
            }
            printf("%f ",DCT[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
    return DCT;
}

float** calcQuantMatrix(int quality)
{
    int i,j,sizeX,sizeY;
    sizeX = sizeY = 8;
    float** outMatrix = (float**)malloc(sizeY * sizeof(float*));
    for (i = 0; i < sizeX; i++) {
        outMatrix[i] = (float*)malloc(sizeX * sizeof(float));
    }

    for (i = 0; i < sizeY; i++) {
        for(j = 0; j < sizeX; j++) {
            outMatrix[i][j] = 1 + ((1 + i + j) * quality);
        }
    }
    return outMatrix;
}

void convertToJpeg (palette_rgb* inputMas, byte_t sizeX, byte_t sizeY)
{
    //long long int i, j;
    //color_YCbCr jpegBlock[8][8];
    /*
   for (i = 0; i < sizeY - sizeY / 8; i--) {
        for (j = 0; j < sizeX / 8) {

        }
    }
    */
}

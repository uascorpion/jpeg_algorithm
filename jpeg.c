#include "jpeg.h"

/*
    Zig-zag ordering elements of sizeX x sizeY array
*/
float* orderZigZag (float** input_mas, byte_t sizeX, byte_t sizeY)
{
   float* output_mas = (float*)malloc(sizeX * sizeY * sizeof(float));

    byte_t x,y,num;
    x = y = num = 0;
    output_mas[num] = input_mas[y][x];
    do {
        if (x < (sizeX-1)) {
            x++; num++;
            output_mas[num] = input_mas[y][x];
        }
        else {
            y++; num++;
            output_mas[num] = input_mas[y][x];
        }

        while (x > 0) {
            if (y == (sizeY-1)) break;
            y++; x--; num++;
            output_mas[num] = input_mas[y][x];
        };

        if (y < (sizeY-1)) {
            y++; num++;
            output_mas[num] = input_mas[y][x];
        }
        else {
            x++; num++;
            output_mas[num] = input_mas[y][x];
        }

        while (y > 0) {
            if (x == (sizeX-1)) break;
            y--; x++; num++;
            output_mas[num] = input_mas[y][x];
        };
    } while (num < (sizeX * sizeY - 1));

    return output_mas;
}

/*
    Converting color scheme from RGB to YCbCr
*/
color_YCbCr convertRGBtoYCbCr (palette_rgb inPixel) {
    color_YCbCr outPixel;
    outPixel.Y = 0.299 * inPixel.red + 0.587 * inPixel.green + 0.114 * inPixel.blue;
    outPixel.Cr = 128 + 0.5 * inPixel.red - 0.418688 * inPixel.green - 0.081312 * inPixel.blue;
    outPixel.Cb = 128 - 0.168736 * inPixel.red - 0.331264 * inPixel.green + 0.5 * inPixel.blue;
    return outPixel;
}

/*
    Dividing image matrix with size sizeX x sizeY in number of matrix with sizes 8 x 8
*/
color_YCbCr* divideImageBySquers(color_YCbCr* mas,int sizeX, int sizeY, int curNumOfSqrX, int curNumOfSqrY, int numOfSqrX)
{
    long long int i,j;
    color_YCbCr* outMas = (color_YCbCr*)malloc(64 * sizeof(color_YCbCr));

    for(j = 0; j < 8; j++) {
        for (i = 0; i < 8; i++) {
            outMas[i + j * 8] = mas[i + curNumOfSqrX * 8 + sizeX * j + curNumOfSqrY * 64 * numOfSqrX];
            (outMas[i + j * 8]).Y -= 128;
            (outMas[i + j * 8]).Cb -= 128;
            (outMas[i + j * 8]).Cr -= 128;
        }
    }
    return outMas;
}

/*
    Calculating of Discrete Cosine Transform matrix for size "8"
*/
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
        }
    }
    return DCT;
}

/*
    Calculating quantization matrix
*/
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

/*
    Proces of converting bitmap image to jpeg image
*/
void convertToJpeg (palette_rgb* inputMas, dword_t sizeX, dword_t sizeY, int quality)
{
    /*
        Converting image to YCbCr
        Input is 24-bit bitmap image vector
     */
    color_YCbCr* resMas = (color_YCbCr*)malloc(sizeX * sizeY * sizeof(color_YCbCr));
    color_YCbCr* masBackup;
    masBackup = resMas;

    color_YCbCr pixel;

    long long int i;
    for (i = 0; i < (sizeX * sizeY); i++)
    {
         pixel = convertRGBtoYCbCr(*inputMas);
         *resMas = pixel;
         *resMas++;
         *inputMas++;
    }
    resMas = masBackup;

    /*
        Calculating DCT matrix and transposed DCT matrix. Them will be used on the next step
    */
    float** DCTmatrix = calculateDCTmatrix();
    float** TranspDCTMatrx = transMatrix(DCTmatrix, 8, 8);

    /*
        Calculating quantization matrix
     */
    float** QuatnMatrix = calcQuantMatrix(quality);

    /*
        Dividing image on squares 8x8 and run compress algorithm for every of them
    */
    int numOfSqrX = sizeX / 8;
    int numOfSqrY = sizeY / 8;

    int curNumOfSqrX = 0;
    int curNumOfSqrY = 0;
    color_YCbCr* curSqr;

    float** curSqrY = (float**) malloc(8 * sizeof(float*));
    for (i = 0; i < 8; i++) {
        curSqrY[i] = (float*) malloc(8 * sizeof(float));
    }

    float** curSqrCb = (float**) malloc(8 * sizeof(float*));
    for (i = 0; i < 8; i++) {
        curSqrCb[i] = (float*) malloc(8 * sizeof(float));
    }

    float** curSqrCr = (float**) malloc(8 * sizeof(float*));
    for (i = 0; i < 8; i++) {
        curSqrCr[i] = (float*) malloc(8 * sizeof(float));
    }

    float* curVectY  = (float*)malloc(64 * sizeof(float));
    float* curVectCb = (float*)malloc(64 * sizeof(float));
    float* curVectCr = (float*)malloc(64 * sizeof(float));

    int k = 0; /* Square counter */

    while (curNumOfSqrY < numOfSqrY)
    {
        while (curNumOfSqrX < numOfSqrX)
        {
            int i,j;
            /*
                Compressing every square
            */
            curSqr = divideImageBySquers(resMas, sizeX, sizeY, curNumOfSqrX, curNumOfSqrY, numOfSqrX);
            for (j = 0; j < 0; j++) {
                for (i = 0; i < 0; i++) {
                  curSqrY [j][i] = (curSqr[8 * j + i]).Y;
                  curSqrCb[j][i] = (curSqr[8 * j + i]).Cb;
                  curSqrCr[j][i] = (curSqr[8 * j + i]).Cr;
                }
            }

            /*
                Calculating Discrete Cosine Transform for every matrix (Y, Cb, Cr)
            */
            curSqrY  = multMatrix(multMatrix(curSqrY,  DCTmatrix, 8), TranspDCTMatrx, 8);
            curSqrCb = multMatrix(multMatrix(curSqrCb, DCTmatrix, 8), TranspDCTMatrx, 8);
            curSqrCr = multMatrix(multMatrix(curSqrCr, DCTmatrix, 8), TranspDCTMatrx, 8);

            /*
                Quantization every matrix (Y, Cb, Cr)
            */
            curSqrY  = divideMatrixByMatrix(curSqrY,  QuatnMatrix);
            curSqrCb = divideMatrixByMatrix(curSqrCb, QuatnMatrix);
            curSqrCr = divideMatrixByMatrix(curSqrCr, QuatnMatrix);

            /*
                ZigZag ordering elements of every matrix (Y, Cb, Cr)
            */
            curVectY  = orderZigZag(curSqrY,  8, 8);
            curVectCb = orderZigZag(curSqrCb, 8, 8);
            curVectCr = orderZigZag(curSqrCr, 8, 8);
            printf("Converted square #%d\n",k++);

            curNumOfSqrX++;
        }
        curNumOfSqrX = 0;
        curNumOfSqrY++;
        printf("\n\n");
    }
}

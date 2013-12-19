#include "jpeg.h"

void print1RGB(palette_rgb* inMas, long long int elnum)
{
    int* Rmas = (int*)malloc(elnum * sizeof(int));
    int* Gmas = (int*)malloc(elnum * sizeof(int));
    int* Bmas = (int*)malloc(elnum * sizeof(int));
    long long int i;
    for (i = 0; i < elnum; i++) {
       Rmas[i]  = (inMas[i]).red;
       Gmas[i]  = (inMas[i]).green;
       Bmas[i]  = (inMas[i]).blue;
    }
    int j;

    printf("\n Red part\n\n");
    for (i = 0; i < (elnum/8); i++) {
        for(j = 0; j < 8; j++) {
            printf("%4d ", Rmas[i * 8 + j]);
        }
        printf("\n");
    }

    printf("\n Green part\n\n");
    for (i = 0; i < (elnum/8); i++) {
        for(j = 0; j < 8; j++) {
            printf("%4d ",Gmas[i * 8 + j]);
        }
        printf("\n");
    }

    printf("\n Blue part\n\n");
    for (i = 0; i < (elnum/8); i++) {
        for(j = 0; j < 8; j++) {
            printf("%4d ",Bmas[i * 8 + j]);
        }
        printf("\n");
    }
    free(Rmas);
    free(Gmas);
    free(Bmas);
}

void print1YCbCr(color_YCbCr* inMas, long long int elnum)
{
    int* Ymas  = (int*)malloc(elnum * sizeof(int));
    int* Cbmas = (int*)malloc(elnum * sizeof(int));
    int* Crmas = (int*)malloc(elnum * sizeof(int));

    long long int i;
    for (i = 0; i < elnum; i++) {
       Ymas [i]  = (inMas[i]).Y;
       Cbmas[i]  = (inMas[i]).Cb;
       Crmas[i]  = (inMas[i]).Cr;
    }
    int j;

    printf("\n Y part\n\n");
    for (i = 0; i < (elnum/8); i++) {
        for(j = 0; j < 8; j++) {
            printf("%4d ", Ymas[i * 8 + j]);
        }
        printf("\n");
    }

    printf("\n Cb part\n\n");
    for (i = 0; i < (elnum/8); i++) {
        for(j = 0; j < 8; j++) {
            printf("%4d ",Cbmas[i * 8 + j]);
        }
        printf("\n");
    }

    printf("\n Cr part\n\n");
    for (i = 0; i < (elnum/8); i++) {
        for(j = 0; j < 8; j++) {
            printf("%4d ",Crmas[i * 8 + j]);
        }
        printf("\n");
    }
    free(Ymas);
    free(Cbmas);
    free(Crmas);
}

/*
    Zig-zag ordering elements of sizeX x sizeY array
*/
int* orderZigZag (float** input_mas, byte_t sizeX, byte_t sizeY)
{
    int* output_mas = (int*)malloc(sizeX * sizeY * sizeof(int));

    byte_t x,y,num;
    x = y = num = 0;
    output_mas[num] = (int)input_mas[y][x];
    do {
        if (x < (sizeX-1)) {
            x++; num++;
            output_mas[num] = (int)input_mas[y][x];
        }
        else {
            y++; num++;
            output_mas[num] = (int)input_mas[y][x];
        }

        while (x > 0) {
            if (y == (sizeY-1)) break;
            y++; x--; num++;
            output_mas[num] = (int)input_mas[y][x];
        };

        if (y < (sizeY-1)) {
            y++; num++;
            output_mas[num] = (int)input_mas[y][x];
        }
        else {
            x++; num++;
            output_mas[num] = (int)input_mas[y][x];
        }

        while (y > 0) {
            if (x == (sizeX-1)) break;
            y--; x++; num++;
            output_mas[num] = (int)input_mas[y][x];
        };
    } while (num < (sizeX * sizeY - 1));

    return output_mas;
}

RLE_mas CodingRLE(int* inputMas)
{
    int* outMas = (int*)malloc(128 * sizeof(int));
    int i;
    int j = 0;
    int zeroCounter = 0;
    for (i = 0; i < 64; i++)
    {
        if (inputMas[i] != 0) {
          outMas[j] = zeroCounter;
          j++;
          outMas[j] = inputMas[i];
          j++;
        }
        else {
                zeroCounter = 0;
                while ((inputMas[i] == 0) && (i < 64)) {
                    i++;
                    zeroCounter++;
                };
                outMas[j] = zeroCounter;
                j++;
                if (i == 64) {
                    outMas[j] = inputMas[i-1];
                }
                else {
                    outMas[j] = inputMas[i];
                    j++;
                }
                zeroCounter = 0;
        }
    }
    //print1dImas(outMas,8, 8);

    RLE_mas returnRes;
    int* resMas = (int*)malloc((j+1)*sizeof(int));
    for (i = 0; i <= j; i++) {
        resMas[i] = outMas[i];
    }

    returnRes.mas = resMas;
    returnRes.MasSize = j+1;
    free(outMas);
    return returnRes;
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
    print1RGB(inputMas, 64);
    /*
        Converting image to YCbCr
        Input is 24-bit bitmap image vector
     */
    color_YCbCr* resMas = (color_YCbCr*)malloc(sizeX * sizeY * sizeof(color_YCbCr));
    color_YCbCr* masBackup;
    masBackup = resMas;

    color_YCbCr pixel;

    long long int i;
    for (i = 0; i < (sizeX * sizeY); i++) {
         pixel = convertRGBtoYCbCr(*inputMas);
         *resMas = pixel;
         *resMas++;
         *inputMas++;
    }
    resMas = masBackup;
    print1YCbCr(resMas, 64);

    /*
        Calculating DCT matrix and transposed DCT matrix. Them will be used on the next step
    */
    float** DCTmatrix = calculateDCTmatrix();
    //printf("\nDCT matrix\n\n");
    //print2dmas(DCTmatrix, 8, 8);
    float** TranspDCTMatrx = transMatrix(DCTmatrix, 8, 8);
    //printf("\nTransp DCT matrix\n\n");
    //print2dmas(TranspDCTMatrx, 8, 8);

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

    int* curVectY  = (int*)malloc(64 * sizeof(int));
    int* curVectCb = (int*)malloc(64 * sizeof(int));
    int* curVectCr = (int*)malloc(64 * sizeof(int));

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
            print1YCbCr(curSqr, 64);
            for (j = 0; j < 8; j++) {
                for (i = 0; i < 8; i++) {
                  curSqrY [j][i] = (curSqr[8 * j + i]).Y;
                  curSqrCb[j][i] = (curSqr[8 * j + i]).Cb;
                  curSqrCr[j][i] = (curSqr[8 * j + i]).Cr;
                }
            }
            //printf("\nDivided image\n");
            //printf("\nCurrent Y square\n\n");
            //print2dFmas(curSqrY,8,8);
            //printf("\nCurrent Cb square\n\n");
            //print2dFmas(curSqrCb,8,8);
            //printf("\nCurrent Cr square\n\n");
            //print2dFmas(curSqrCr,8,8);

            /*
                Calculating Discrete Cosine Transform for every matrix (Y, Cb, Cr)
            */
            curSqrY  = multMatrix(DCTmatrix, multMatrix(curSqrY, TranspDCTMatrx, 8), 8);
            curSqrCb = multMatrix(DCTmatrix, multMatrix(curSqrCb, TranspDCTMatrx, 8), 8);
            curSqrCr = multMatrix(DCTmatrix, multMatrix(curSqrCr, TranspDCTMatrx, 8), 8);

            //printf("\nDCT matrixes\n");
            //printf("\nCurrent Y square\n\n");
            //print2dFmas(curSqrY,8,8);
            //printf("\nCurrent Cb square\n\n");
            //print2dFmas(curSqrCb,8,8);
            //printf("\nCurrent Cr square\n\n");
            //print2dFmas(curSqrCr,8,8);

            /*
                Quantization every matrix (Y, Cb, Cr)
            */

            //float QuatnYMatrix[8][8] = {{16,11,10,16,24,40,51,61},{12,12,14,19,26,58,60,55},{14,13,16,24,40,57,69,56},{14,17,22,29,51,87,80,62},{18,22,37,56,68,109,103,77},{24,35,55,64,81,104,113,92},{49,64,78,87,103,121,120,101},{72,92,95,98,112,100,103,99}};
            curSqrY  = divideMatrixByMatrix(curSqrY,  QuatnMatrix);
            curSqrCb = divideMatrixByMatrix(curSqrCb, QuatnMatrix);
            curSqrCr = divideMatrixByMatrix(curSqrCr, QuatnMatrix);

            printf("\n\nQuantizen matrixes\n");
            printf("\n\nCurrent Y square\n\n");
            print2dFmas(curSqrY,8,8);
            printf("\n\nCurrent Cb square\n\n");
            print2dFmas(curSqrCb,8,8);
            printf("\n\nCurrent Cr square\n\n");
            print2dFmas(curSqrCr,8,8);

            /*
                ZigZag ordering elements of every matrix (Y, Cb, Cr)
            */
            curVectY  = orderZigZag(curSqrY,  8, 8);
            curVectCb = orderZigZag(curSqrCb, 8, 8);
            curVectCr = orderZigZag(curSqrCr, 8, 8);
            printf("\n\nCurrent Y vector\n\n");
            print1dImas(curVectY,64);
            printf("\n\nCurrent Cb vector\n\n");
            print1dImas(curVectCb,64);
            printf("\n\nCurrent Cr vector\n\n");
            print1dImas(curVectCr,64);
            //printf("Converted square #%d\n",k++);

            /*
                RLE-coding of every matrix (Y, Cb, Cr)
            */

            RLE_mas rleY = CodingRLE(curVectY);
            RLE_mas rleCb = CodingRLE(curVectCb);
            RLE_mas rleCr = CodingRLE(curVectCr);
            printf("\n\nRLE_Y_matrix\n\n");
            print1dImas(rleY.mas, rleY.MasSize);
            printf("\n\nRLE_Cb_matrix\n\n");
            print1dImas(rleCb.mas, rleCb.MasSize);
            printf("\n\nRLE_Cr_matrix\n\n");
            print1dImas(rleCr.mas, rleCr.MasSize);

            curNumOfSqrX++;
        }
        curNumOfSqrX = 0;
        curNumOfSqrY++;
        printf("\n\n");
    }
}

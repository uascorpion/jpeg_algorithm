#include "jpeg.h"
#include "jpeg_additional.h"

/* jpegsnoop */

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

/*
    RLE coding of AC elements
*/
RLE_mas CodingRLE(int* inputMas)
{
    int* outMas = (int*)malloc(128 * sizeof(int));
    int i;
    int j = 0;
    int zeroCounter = 0;
    for (i = 1; i < 64; i++)
    {
        if (inputMas[i] != 0) {
          outMas[j] = zeroCounter;
          j++;
          outMas[j] = inputMas[i];
          j++;
        }
        else {
            zeroCounter = 1;
            while ((inputMas[i] == 0) && (i < 63)) {
                i++;
                zeroCounter++;
                /*if (zeroCounter == 15) {
                    outMas[j] = zeroCounter;
                    j++;
                    outMas[j] = inputMas[i];
                    j++;
                    zeroCounter = 0;
                }*/
            };
            outMas[j] = zeroCounter;
            j++;
            outMas[j] = inputMas[i];
            j++;
            zeroCounter = 0;
        }
    }
    //print1dImas(outMas,8, 8);

    RLE_mas returnRes;
    int* resMas = (int*)malloc((j)*sizeof(int));
    for (i = 0; i < j; i++) {
        resMas[i] = outMas[i];
    }

    returnRes.mas = resMas;
    returnRes.MasSize = j;
    free(outMas);
    return returnRes;
}

/*
    Huffman coding of DC elements
*/
char_mas codingDC(int inputDC)
{
    int row, column, i;
    row = column = 0;
    for (i = 0; i < 16; i++) {
        /* for positive inputDC only */
        if (inputDC > 0) {
            if ((pow(2, i) - 1) >= inputDC) {
                row = i;
                column = inputDC;
                break;
            }
        }
        /* for negative inputDC */
        else {
            if ((-(pow(2, i)-1)) <= inputDC) {
                row = i;
                column = inputDC + (pow(2, i)-1);
                break;
            }
        }
    }
    printf("\nRow = %d \n", row);
    printf("\nColumn = %d\n", column);
    char* res = (char*)malloc((row + 2) * sizeof(char));
    for (i = 0; i <= row; i++) {
        if (i < row) {
            res[i] = '1';
        }
        else {
            res[i] = '0';
        }
    }
    res[row + 1] = '\0';

    char* bincode = dec2bin(column, row);

    char_mas charRes;
    charRes.mas = strcat(res, bincode);
    charRes.MasSize = strlen(charRes.mas);
    return charRes;
}


RC_pair getRCpair(int inputAC)
{
    int row, column, i;
    row = column = 0;
    for (i = 0; i < 16; i++) {
        /* for positive inputAC only */
        if (inputAC > 0) {
            if ((pow(2, i) - 1) >= inputAC) {
                row = i;
                column = inputAC;
                break;
            }
        }
        /* for negative inputAC */
        else {
            if ((-(pow(2, i)-1)) <= inputAC) {
                row = i;
                column = inputAC + (pow(2, i)-1);
                break;
            }
        }
    }
    RC_pair res;
    res.R = row;
    res.C = column;
    return res;
}

/*
    Get Huffman code by using table that recomended by jpeg standart
*/
char_mas Huffman(int Relem, int Zelem, bool isY)
{
    /* Huffman code table for Y matrixes from jpeg standart */
    char HuffmY[160][17] = {{"00\0"},{"01\0"},{"100\0"},{"1011\0"},{"11010\0"},
                           {"1111000\0"},{"11111000\0"},{"1111110110\0"},{"1111111110000010\0"},{"1111111110000011\0"},
                           {"1100\0"},{"11011\0"},{"11110001\0"},{"111110110\0"},{"11111110110\0"},
                           {"1111111110000100\0"},{"1111111110000101\0"},{"1111111110000110\0"},{"1111111110000111\0"},{"1111111110001000\0"},
                           {"11100\0"},{"11111001\0"},{"1111110111\0"},{"111111110100\0"},{"111111110001001\0"},
                           {"111111110001010\0"},{"111111110001011\0"},{"111111110001100\0"},{"111111110001101\0"},{"111111110001110\0"},
                           {"111010\0"},{"111110111\0"},{"111111110101\0"},{"1111111110001111\0"},{"1111111110010000\0"},
                           {"1111111110010001\0"},{"1111111110010010\0"},{"1111111110010011\0"},{"1111111110010100\0"},{"1111111110010101\0"},
                           {"111011\0"},{"1111111000\0"},{"1111111110010110\0"},{"1111111110010111\0"},{"1111111110011000\0"},
                           {"1111111110011001\0"},{"1111111110011010\0"},{"1111111110011011\0"},{"1111111110011100\0"},{"1111111110011101\0"},
                           {"1111010\0"},{"11111110111\0"},{"1111111110011110\0"},{"1111111110011111\0"},{"1111111110100000\0"},
                           {"1111111110100001\0"},{"1111111110100010\0"},{"1111111110100011\0"},{"1111111110100100\0"},{"1111111110100101\0"},
                           {"1111011\0"},{"111111110110\0"},{"1111111110100110\0"},{"1111111110100111\0"},{"111111110101000\0"},
                           {"1111111110101001\0"},{"1111111110101010\0"},{"1111111110101011\0"},{"1111111110101100\0"},{"1111111110101101\0"},
                           {"11111010\0"},{"111111110111\0"},{"1111111110101110\0"},{"1111111110101111\0"},{"1111111110110000\0"},
                           {"1111111110110001\0"},{"1111111110110010\0"},{"1111111110110011\0"},{"1111111110110100\0"},{"1111111110110101\0"},
                           {"111111000\0"},{"111111111000000\0"},{"1111111110110110\0"},{"1111111110110111\0"},{"1111111110111000\0"},
                           {"1111111110111001\0"},{"1111111110111010\0"},{"1111111110111011\0"},{"1111111110111100\0"},{"1111111110111101\0"},
                           {"111111001\0"},{"1111111110111110\0"},{"1111111110111110\0"},{"1111111111000000\0"},{"1111111111000001\0"},
                           {"1111111111000010\0"},{"1111111111000011\0"},{"1111111111000011\0"},{"1111111111000101\0"},{"1111111111000110\0"},
                           {"111111010\0"},{"1111111111000111\0"},{"1111111111001000\0"},{"1111111111001001\0"},{"1111111111001010\0"},
                           {"1111111111001011\0"},{"1111111111001100\0"},{"1111111111001101\0"},{"1111111111001110\0"},{"1111111111001111\0"},
                           {"1111111001\0"},{"1111111111010000\0"},{"1111111111010001\0"},{"1111111111010010\0"},{"1111111111010011\0"},
                           {"1111111111010100\0"},{"1111111111010101\0"},{"1111111111010110\0"},{"1111111111010111\0"},{"1111111111011000\0"},
                           {"1111111010\0"},{"1111111111011001\0"},{"1111111111011010\0"},{"1111111111011011\0"},{"1111111111011100\0"},
                           {"1111111111011101\0"},{"1111111111011110\0"},{"1111111111011111\0"},{"1111111111100000\0"},{"1111111111100001\0"},
                           {"11111111000\0"},{"1111111111100010\0"},{"1111111111100011\0"},{"1111111111100100\0"},{"1111111111100101\0"},
                           {"1111111111100110\0"},{"1111111111100111\0"},{"1111111111101000\0"},{"1111111111101001\0"},{"1111111111101010\0"},
                           {"1111111111101011\0"},{"1111111111101100\0"},{"1111111111101101\0"},{"1111111111101110\0"},{"1111111111101111\0"},
                           {"1111111111110000\0"},{"1111111111110001\0"},{"1111111111110010\0"},{"1111111111110011\0"},{"1111111111110100\0"},
                           {"11111111001\0"},{"1111111111110101\0"},{"1111111111110110\0"},{"1111111111110111\0"},{"1111111111111000\0"},
                           {"1111111111111001\0"},{"1111111111111010\0"},{"1111111111111011\0"},{"1111111111111101\0"},{"1111111111111110\0"}};

    /* Huffman code table for Cb and Cr matrixes from jpeg standart */
    char HuffmC[160][17] = {{"01\0"},{"100\0"},{"1010\0"},{"11000\0"},{"11001\0"},
                           {"111000\0"},{"1111000\0"},{"111110100\0"},{"1111110110\0"},{"111111110100\0"},
                           {"1011\0"},{"111001\0"},{"11110110\0"},{"111110101\0"},{"11111110110\0"},
                           {"111111110101\0"},{"111111110001000\0"},{"111111110001001\0"},{"111111110001010\0"},{"111111110001011\0"},
                           {"11010\0"},{"11110111\0"},{"1111110111\0"},{"111111110110\0"},{"111111111000010\0"},
                           {"1111111110001100\0"},{"1111111110001101\0"},{"1111111110001110\0"},{"1111111110001111\0"},{"1111111110010000\0"},
                           {"11011\0"},{"11111000\0"},{"1111111000\0"},{"111111110111\0"},{"1111111110010001\0"},
                           {"1111111110010010\0"},{"1111111110010011\0"},{"1111111110010100\0"},{"1111111110010101\0"},{"1111111110010110\0"},
                           {"111010\0"},{"111110110\0"},{"1111111110010111\0"},{"1111111110011000\0"},{"1111111110011001\0"},
                           {"1111111110011010\0"},{"1111111110011011\0"},{"1111111110011100\0"},{"1111111110011101\0"},{"1111111110011110\0"},
                           {"111011\0"},{"1111111001\0"},{"1111111110011111\0"},{"1111111110100000\0"},{"1111111110100001\0"},
                           {"1111111110100010\0"},{"1111111110100011\0"},{"1111111110100100\0"},{"1111111110100101\0"},{"1111111110100110\0"},
                           {"1111001\0"},{"11111110111\0"},{"1111111110100111\0"},{"1111111110101000\0"},{"1111111110101001\0"},
                           {"1111111110101010\0"},{"1111111110101011\0"},{"1111111110101100\0"},{"1111111110101101\0"},{"1111111110101110\0"},
                           {"1111010\0"},{"11111111000\0"},{"1111111110101111\0"},{"1111111110110000\0"},{"1111111110110001\0"},
                           {"1111111110110010\0"},{"1111111110110011\0"},{"1111111110110100\0"},{"1111111110110101\0"},{"1111111110110110\0"},
                           {"11111001\0"},{"1111111110110111\0"},{"1111111110111000\0"},{"1111111110111001\0"},{"1111111110111010\0"},
                           {"1111111110111011\0"},{"1111111110111100\0"},{"1111111110111101\0"},{"1111111110111110\0"},{"1111111110111111\0"},
                           {"111110111\0"},{"1111111111000000\0"},{"1111111111000001\0"},{"1111111111000010\0"},{"1111111111000011\0"},
                           {"1111111111000100\0"},{"1111111111000101\0"},{"1111111111000110\0"},{"1111111111000111\0"},{"1111111111001000\0"},
                           {"111111000\0"},{"1111111111001001\0"},{"1111111111001010\0"},{"1111111111001011\0"},{"1111111111001100\0"},
                           {"1111111111001101\0"},{"1111111111001110\0"},{"1111111111001111\0"},{"1111111111010000\0"},{"1111111111010001\0"},
                           {"111111001\0"},{"1111111111010010\0"},{"1111111111010011\0"},{"1111111111010100\0"},{"1111111111010101\0"},
                           {"1111111111010110\0"},{"1111111111010111\0"},{"1111111111011000\0"},{"1111111111011001\0"},{"1111111111011010\0"},
                           {"111111010\0"},{"1111111111011011\0"},{"1111111111011100\0"},{"1111111111011101\0"},{"1111111111011110\0"},
                           {"1111111111011111\0"},{"1111111111100000\0"},{"1111111111100001\0"},{"1111111111100010\0"},{"1111111111100011\0"},
                           {"11111111001\0"},{"1111111111100100\0"},{"1111111111100101\0"},{"1111111111100110\0"},{"1111111111100111\0"},
                           {"1111111111101000\0"},{"1111111111101001\0"},{"1111111111101010\0"},{"1111111111101011\0"},{"1111111111101100\0"},
                           {"11111111100000\0"},{"1111111111101101\0"},{"1111111111101110\0"},{"1111111111101111\0"},{"1111111111110000\0"},
                           {"1111111111110001\0"},{"1111111111110010\0"},{"1111111111110011\0"},{"1111111111110100\0"},{"1111111111110101\0"},
                           {"111111111000011\0"},{"111111111010110\0"},{"1111111111110111\0"},{"1111111111111000\0"},{"1111111111111001\0"},
                           {"1111111111111010\0"},{"1111111111111011\0"},{"1111111111111100\0"},{"1111111111111101\0"},{"1111111111111110\0"}};

    char_mas element;
    if (isY) {
        element.mas = HuffmY[10 * Zelem + Relem - 1];
        element.MasSize = strlen(HuffmY[10 * Zelem + Relem - 1]);
    }
    else {
        element.mas = HuffmC[10 * Zelem + Relem - 1];
        element.MasSize = strlen(HuffmC[10 * Zelem + Relem - 1]);
    }
    return element;
}

/*
    Huffman coding of AC elements
*/
char_mas codingAC(int inputAC,int inputZ, bool isY)
{
    int R,C;
    RC_pair RCpair = getRCpair(inputAC);
    R = RCpair.R;
    C = RCpair.C;

    char_mas result;
    result.mas = strcat(Huffman(R, inputZ, isY).mas, dec2bin(C, R));
    result.MasSize = strlen(result.mas);

    return result;
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
float** calcDCTmatrix(void)
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
    masBackup  = resMas;

    color_YCbCr pixel;

    long long int i;
    for (i = 0; i < (sizeX * sizeY); i++) {
         pixel = convertRGBtoYCbCr(*inputMas);
         *resMas = pixel;
         *resMas++;
         *inputMas++;
    }
    //free(inputMas);
    resMas = masBackup;
    print1YCbCr(resMas, 64);

    /*
        Calculating DCT matrix and transposed DCT matrix. Them will be used on the next step
    */
    float** DCTmatrix = calcDCTmatrix();
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

    color_YCbCr* curSqr = (color_YCbCr*)malloc(64 * sizeof(color_YCbCr));

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

    int k = 0;

    /*
        For storing DC-elements (current and prewious)
    */
    int curDCYelement   = 0;
    int prewDCYelement  = 0;
    int curDCCbelement  = 0;
    int prewDCCbelement = 0;
    int curDCCrelement  = 0;
    int prewDCCrelement = 0;

    /*
        Compressing every square
    */
    while (curNumOfSqrY < numOfSqrY)
    {
        while (curNumOfSqrX < numOfSqrX)
        {
            int i,j;
            /*
                Get current block from all image
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
/*
            printf("\nDivided image\n");
            printf("\nCurrent Y square\n\n");
            print2dFmas(curSqrY,8,8);
            printf("\nCurrent Cb square\n\n");
            print2dFmas(curSqrCb,8,8);
            printf("\nCurrent Cr square\n\n");
            print2dFmas(curSqrCr,8,8);
*/
            /*
                Calculating Discrete Cosine Transform for every matrix (Y, Cb, Cr)
            */
            curSqrY  = multMatrix(DCTmatrix, multMatrix(curSqrY,  TranspDCTMatrx, 8), 8);
            curSqrCb = multMatrix(DCTmatrix, multMatrix(curSqrCb, TranspDCTMatrx, 8), 8);
            curSqrCr = multMatrix(DCTmatrix, multMatrix(curSqrCr, TranspDCTMatrx, 8), 8);
/*
            printf("\nDCT matrixes\n");
            printf("\nCurrent Y square\n\n");
            print2dFmas(curSqrY,8,8);
            printf("\nCurrent Cb square\n\n");
            print2dFmas(curSqrCb,8,8);
            printf("\nCurrent Cr square\n\n");
            print2dFmas(curSqrCr,8,8);
*/
            /*
                Quantization every matrix (Y, Cb, Cr)
            */
/*
            //Quant matrix for Y part, recomended byjped standart
            float QuantYMatrix[8][8] = {{16,11,10,16,24,40,51,61},{12,12,14,19,26,58,60,55},
                                        {14,13,16,24,40,57,69,56},{14,17,22,29,51,87,80,62},
                                        {18,22,37,56,68,109,103,77},{24,35,55,64,81,104,113,92},
                                        {49,64,78,87,103,121,120,101},{72,92,95,98,112,100,103,99}};
*/
            curSqrY  = divideMatrixByMatrix(curSqrY,  QuatnMatrix);
            curSqrCb = divideMatrixByMatrix(curSqrCb, QuatnMatrix);
            curSqrCr = divideMatrixByMatrix(curSqrCr, QuatnMatrix);
/*
            printf("\n\nQuantizen matrixes\n");
            printf("\n\nCurrent Y square\n\n");
            print2dFmas(curSqrY,8,8);
            printf("\n\nCurrent Cb square\n\n");
            print2dFmas(curSqrCb,8,8);
            printf("\n\nCurrent Cr square\n\n");
            print2dFmas(curSqrCr,8,8);
*/
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

            /*
                RLE-coding of every matrix (Y, Cb, Cr) /Only ACi elements/
            */
            RLE_mas rleY  = CodingRLE(curVectY);
            int* masRLECoddedY = (int*)malloc(rleY.MasSize*sizeof(int));
            masRLECoddedY = rleY.mas;
            int masRLECoddedYCnt = rleY.MasSize;

            RLE_mas rleCb = CodingRLE(curVectCb);
            int* masRLECoddedCb = (int*)malloc(rleCb.MasSize*sizeof(int));
            masRLECoddedCb = rleCb.mas;
            int masRLECoddedCbCnt = rleCb.MasSize;

            RLE_mas rleCr = CodingRLE(curVectCr);
            int* masRLECoddedCr = (int*)malloc(rleCr.MasSize*sizeof(int));
            masRLECoddedCr = rleCr.mas;
            int masRLECoddedCrCnt = rleCr.MasSize;

            printf("\n\nRLE_Y_matrix\n\n");
            print1dImas(masRLECoddedY, masRLECoddedYCnt);
            printf("\n\nRLE_Cb_matrix\n\n");
            print1dImas(masRLECoddedCb, masRLECoddedCbCnt);
            printf("\n\nRLE_Cr_matrix\n\n");
            print1dImas(masRLECoddedCr, masRLECoddedCrCnt);

            /*
                Get DC element of every matrix
            */
            curDCYelement  = curVectY [0] - prewDCYelement;
            curDCCbelement = curVectCb[0] - prewDCCbelement;
            curDCCrelement = curVectCr[0] - prewDCCrelement;

            /*
                Huffman codding DC element of every matrix
            */
            char_mas coddedYDC  = codingDC(curDCYelement);
            char* coddedYDCval = (char*)malloc(coddedYDC.MasSize * sizeof(char));
            coddedYDCval = coddedYDC.mas;
            int coddedYDClen = coddedYDC.MasSize;

            char_mas coddedCbDC = codingDC(curDCCbelement);
            char* coddedCbDCval = (char*)malloc(coddedCbDC.MasSize * sizeof(char));
            coddedCbDCval = coddedCbDC.mas;
            int coddedCbDClen = coddedCbDC.MasSize;

            char_mas coddedCrDC = codingDC(curDCCrelement);
            char* coddedCrDCval = (char*)malloc(coddedCrDC.MasSize * sizeof(char));
            coddedCrDCval = coddedCrDC.mas;
            int coddedCrDClen = coddedCrDC.MasSize;

            printf("\nY_DC\n");
            print1dCmas(coddedYDCval, coddedYDClen);

            printf("\nCb_DC\n");
            print1dCmas(coddedCbDCval, coddedCbDClen);

            printf("\nCr_DC\n");
            print1dCmas(coddedCrDCval, coddedCrDClen);

            /*
                Huffman codding of every AC elenet of every matrix and concatenate result with codded DC element
            */
            char* coddedY = (char*)malloc(coddedYDClen * sizeof(char));
            strcpy(coddedY, coddedYDCval);
            int coddedYlen = coddedYDClen;
            int AC, Z;
            for (k = 1; k < masRLECoddedYCnt; k++)
            {
                AC = masRLECoddedY[k];
                Z  = masRLECoddedY[k-1];

                char_mas elementYAC = codingAC(AC, Z, true);

                coddedYlen += elementYAC.MasSize;
                coddedY = (char*)realloc(coddedY, coddedYlen * sizeof(char));
                strcat(coddedY, elementYAC.mas);
                k++;
            }
/*
            char* coddedCb = (char*)malloc(coddedCbDClen * sizeof(char));
            strcpy(coddedY, coddedCbDCval);
            int coddedCblen = coddedCbDClen;
            for (k = 1; k < masRLECoddedCbCnt; k++)
            {
                AC = masRLECoddedCb[k];
                Z  = masRLECoddedCb[k-1];

                char_mas elementCbAC = codingAC(AC, Z, false);

                coddedCblen += elementCbAC.MasSize;
                coddedCb = (char*)realloc(coddedCb, coddedCblen * sizeof(char));
                strcat(coddedCb, elementCbAC.mas);
                k++;
            }

            char_mas coddedCrAC;
            for (k = 1; k < rleCr.MasSize; k++)
            {
                AC = rleCr.mas[k];
                Z  = rleCr.mas[k-1];
                char_mas elementCrAC = codingAC(AC, Z, false);
                strcat(coddedCrAC.mas, elementCrAC.mas);
                coddedCrAC.MasSize += elementCrAC.MasSize;
                k++;
            }
*/
            printf("\n_Codded_Y_\n");
            print1dCmas(coddedY, coddedYlen);
/*
            printf("\n_Codded_Cb_\n");
            print1dCmas(coddedCb, coddedCblen);

            printf("\n_Codded_Cr_\n");
            print1dCmas(coddedCr.mas, coddedCr.MasSize);
*/
            prewDCYelement  = curVectY[0];
            prewDCCbelement = curVectCb[0];
            prewDCCrelement = curVectCr[0];
            curNumOfSqrX++;
        }
        curNumOfSqrX = 0;
        curNumOfSqrY++;
        printf("\n\n");
    }
    free(curSqrY);
    free(curSqrCb);
    free(curSqrCr);
    free(resMas);
    free(curVectY);
    free(curVectCb);
    free(curVectCr);
}

#include "jpeg_additional.h"
/*
    Printing all elemens of array of palette_rgb elements
*/
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
            printf("%4d ", Gmas[i * 8 + j]);
        }
        printf("\n");
    }

    printf("\n Blue part\n\n");
    for (i = 0; i < (elnum/8); i++) {
        for(j = 0; j < 8; j++) {
            printf("%4d ", Bmas[i * 8 + j]);
        }
        printf("\n");
    }
    free(Rmas);
    free(Gmas);
    free(Bmas);
}

/*
    Printing all elemens of array of color_YCbCr elements
*/
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
    Converting decimal value to binary (stored in char* array)
*/
char* dec2bin(int n, int codeLong)
{
   int c, d, counter;
   char* pointer = (char*)malloc(codeLong+1);
   counter = 0;
   for ( c = (codeLong-1) ; c >= 0 ; c-- )
   {
      d = n >> c;
      if ( d & 1 )
         *(pointer+counter) = 1 + '0';
      else
         *(pointer+counter) = 0 + '0';
      counter++;
   }
   *(pointer+counter) = '\0';
   return  pointer;
}

/*
    Converting binary value (stored in char* array) to decimal value
*/
long long int bin2dec(char *bin)
{
    int b , k, n;
    int len = 0;
    long long int sum = 0;
    len = strlen(bin) - 1;
    for(k = 0; k <= len; k++) {
        b = 1;
        n = (bin[k] - '0'); /* char to numeric value */
        if ((n > 1) || (n < 0)) {
            puts("\n\n ERROR! BINARY has only 1 and 0!\n");
            return 0;
        }
        b = b << (len - k);
        /* sum it up */
        sum = sum + n * b;
    }
    return sum;
}

void writeJpeg(char* imagestream,int Imgsize, char* imagename)
{
    int fd_out = creat(imagename, PERMS);
    write(fd_out, imagestream, Imgsize);
    close(fd_out);
}

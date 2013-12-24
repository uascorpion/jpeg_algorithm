#include "bmp.h"
#include "jpeg.h"
#include "matrix.h"

int main()
{

    int fd = open("melexis.bmp",O_RDONLY,0);    /* Creationg file descriptor */

    if (-1 == fd) {
        printf("Can't open file\n");
    }

    else {
        palette_rgb* bmpimg = bmp_parse(fd);
        convertToJpeg(bmpimg, img_width, img_height, 3);
    }
/*
    char* testbin2dec = "10100000111110101100010111111001\0";
    long long int result = bin2dec(testbin2dec);
    printf("Result = %d\n", result);

    char_mas mas = codingAC(-1, 13, true);
    print1dCmas(mas.mas,mas.MasSize);
    codingDC(1118);
*/
    return 0;
}

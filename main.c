#include "bmp.h"
#include "jpeg_omp.h"
#include "matrix.h"
#include <stdio.h>

#if defined _USEOPENMP
int thread_number;
#endif

int main()
{
    char filename[80];
    printf("\n Enter file name = ");
    gets(filename);

    printf("\n Enter commpression value = ");
    int compression;
    scanf("%d", &compression);

#if defined _USEOPENMP
    printf("\n\n Enter number of threads = ");
    scanf("%d", &thread_number);
#endif //USEOPENMP

    int fd = open(filename, O_RDONLY, 0);    // Creationg file descriptor

    if (-1 == fd) {
        printf("Can't open file\n");
    }

    else {
        palette_rgb* bmpimg = bmp_parse(fd);
        convertToJpeg(bmpimg, img_width, img_height, compression);
    }

    return 0;
}

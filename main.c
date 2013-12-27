#include "bmp.h"
#include "jpeg.h"
#include "matrix.h"


#if defined _USEOPENMP
int thread_number;
#endif

#if defined _USEMPI
#include <mpi.h>
#endif //define

int main()
{
    int i = 0;
    char* filename = (char*)calloc(80, sizeof(char));
    int compression;
#if defined _USEOPENMP
    FILE* ptr = fopen("config_omp.txt","rt");
#elifdef _USEMPI
    FILE* ptr = fopen("config_mpi.txt","rt");
#else
    FILE* ptr = fopen("config.txt","rt");
#endif // defined

    if (NULL == ptr) {
        printf("Can't open config file\n");
    }
    else {
#if defined _USEOPENMP
        while (fscanf (ptr, "%s%u%u", filename, &compression, &thread_number) != EOF) {
            i++;
            int fd = open(filename, O_RDONLY, 0);
            if (-1 == fd) {
                printf("Can't open file\n");
            }
            else {
                palette_rgb* bmpimg = bmp_parse(fd);
                convertToJpeg(bmpimg, img_width, img_height, compression);
            }
        }
#elifdef _USEMPI

#else
        while (fscanf (ptr, "%s%u", filename, &compression) != EOF) {
            i++;
            int fd = open(filename, O_RDONLY, 0);
            if (-1 == fd) {
                printf("Can't open file\n");
            }
            else {
                palette_rgb* bmpimg = bmp_parse(fd);
                convertToJpeg(bmpimg, img_width, img_height, compression);
            }
        }
#endif // defined
    fclose(ptr);
    }
    return 0;
}

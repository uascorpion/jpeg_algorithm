#include "bmp.h"

/* Reading BMP File header info */
static void bmpFileHeaderRead(int fd)
{
    int * cur_file_adr;     /* Pointer for address in bitmap file */

    /* Reading bitmap info file header */
    cur_file_adr = (int *) (&cur_file_header);
    lseek(fd, 0U, FROM_START);    /* lseek to the start of file */
    read (fd, cur_file_adr, sizeof (BMPfileHeader) );
}

/* Reading BMP image header info */
static void bmpImageInfoHeaderRead(int fd)
{
        int * cur_file_adr;     /* Pointer for address in bitmap file */

        /* Reading bitmap image info header */
        cur_file_adr = (int *) (&cur_info_header);
        lseek(fd, sizeof(BMPfileHeader), FROM_START);    /* lseek to the start of file */
        read (fd, cur_file_adr, sizeof (BMPinfoHeader) );

        /* Get current image parameters for manipulation with them */
        img_height = abs(cur_info_header.biHeight);
        img_width = cur_info_header.biWidth;

        img_bitCount = cur_info_header.biBitCount;  /* Number of bits in image */
        printf("This is %d-bit image\n", img_bitCount);

        img_compression = cur_info_header.biCompression;
        orientation = (cur_info_header.biHeight > 0) ? MIRROR : NORMAL;
        printf("Orientation is %s ", (orientation == MIRROR) ? "MIRRORED\n" : "NORMAL\n");

        /* Reading image matrix (as vector) */
        image_dots = img_height * img_width;    /* count of dots */
        printf("Numner of pixels = %d\n", image_dots);
}

/* Parsing bmp-file for getting image matrix in 24-bit format */
palette_rgb* bmp_parse(int fd)
{
    bmpFileHeaderRead(fd);
    bmpImageInfoHeaderRead(fd);
        int * cur_file_adr;     /* Pointer for address in bitmap file */
        if (1 == img_bitCount) {
            /* TODO */
            close(fd);
            return NULL;
        }

        else if (4 == img_bitCount) {
            lseek(fd, (sizeof(BMPfileHeader) + cur_info_header.biSize), FROM_START);    /* lseek to the palette */
            palette_8_bit palette[16];    /* biClrUsed is not always filled */

            cur_file_adr = (int *) (&palette);
            read(fd, cur_file_adr, sizeof(palette) );   /* read color palette */

            /* Read current image martix */
            byte_t *bitmap_image_pointer_start;
            byte_t *bitmap_image_pointer = malloc(image_dots / 2);  /* every byte is 2 pixels */
            /* Backup pointer to the start of image for free memory in future */
            bitmap_image_pointer_start = bitmap_image_pointer;

            lseek(fd, (cur_file_header.bfOffBits), FROM_START);
            read(fd, bitmap_image_pointer, (image_dots / 2) );

            printf("Read_OK\n");

            /* Converting  to 24-bit image */
            palette_rgb *new_bitmap_image_pointer = malloc(image_dots * sizeof(palette_rgb));
            palette_rgb *new_bitmap_image_pointer_start;
            /* Backup pointer to the start of image for free memory in future */
            new_bitmap_image_pointer_start = new_bitmap_image_pointer;
            dword_t i;
            palette_rgb newpixel;
            for (i = 0; i < image_dots; i++) {
                newpixel.blue  = palette[(byte_t)(((*(bitmap_image_pointer )) & 0xF0) >> 8)].blue;
                newpixel.green = palette[(byte_t)(((*(bitmap_image_pointer )) & 0xF0) >> 8)].green;
                newpixel.red   = palette[(byte_t)(((*(bitmap_image_pointer )) & 0xF0) >> 8)].red;

                *new_bitmap_image_pointer = newpixel;
                *new_bitmap_image_pointer++;

                newpixel.blue  = palette[((*(bitmap_image_pointer )) & 0x0F)].blue;
                newpixel.green = palette[((*(bitmap_image_pointer )) & 0x0F)].green;
                newpixel.red   = palette[((*(bitmap_image_pointer )) & 0x0F)].red;

                *new_bitmap_image_pointer = newpixel;
                *new_bitmap_image_pointer++;
                *bitmap_image_pointer++;
            }

            /* Clearing memory */
            new_bitmap_image_pointer = new_bitmap_image_pointer_start;
            bitmap_image_pointer = bitmap_image_pointer_start;
            //free(new_bitmap_image_pointer);
            free(bitmap_image_pointer);
            close(fd);
            return new_bitmap_image_pointer;
        }

        else if (8 == img_bitCount) {
            /*
             * There are less than 256 colors in color palette.
             * Every BYTE of image save an index from palette for one pixel
             */
            lseek(fd, (sizeof(BMPfileHeader) + cur_info_header.biSize), FROM_START);    /* lseek to the palette */
            palette_8_bit palette[/*cur_info_header.biClrUsed*/256];    /* biClrUsed is not always filled */

            cur_file_adr = (int *) (&palette);
            read(fd, cur_file_adr, sizeof(palette) );   /* read color palette */

            /* Read current image martix */
            byte_t *bitmap_image_pointer_start;
            byte_t *bitmap_image_pointer = malloc(image_dots);
            /* Backup pointer to the start of image for free memory in future */
            bitmap_image_pointer_start = bitmap_image_pointer;

            lseek(fd, (cur_file_header.bfOffBits), FROM_START);
            read(fd, bitmap_image_pointer, image_dots );

            printf("Read_OK\n");

            /* Converting  to 24-bit image */
            palette_rgb *new_bitmap_image_pointer = malloc(image_dots * sizeof(palette_rgb));
            palette_rgb *new_bitmap_image_pointer_start;

            //start of image matrix, used in rotation operation
            palette_rgb *bmpImgStrt;
            bmpImgStrt = new_bitmap_image_pointer;
            //

            /* Backup pointer to the start of image for free memory in future */
            new_bitmap_image_pointer_start = new_bitmap_image_pointer;
            dword_t i;
            palette_rgb newpixel;
            for (i = 0; i < image_dots; i++) {
                newpixel.blue  = palette[(*(bitmap_image_pointer ))].blue;
                newpixel.green = palette[(*(bitmap_image_pointer ))].green;
                newpixel.red   = palette[(*(bitmap_image_pointer ))].red;

                *new_bitmap_image_pointer = newpixel;
                *new_bitmap_image_pointer++;
                *bitmap_image_pointer++;
            }

            //end of image matrix, used in rotation operation
            palette_rgb *bmpImgEnd;
            bmpImgEnd = new_bitmap_image_pointer;
            //

            /* Writting new bmp-file with 24-bit image */
/*
            int fd_out = creat("LENA512_2.BMP",PERMS);
            BMPfileHeader new_file_header;
            BMPinfoHeader new_info_header;
            new_file_header = cur_file_header;
            new_file_header.bfSize = sizeof(BMPfileHeader) + sizeof(BMPinfoHeader) + sizeof(palette_rgb) * image_dots;
            new_file_header.bfOffBits = sizeof(BMPfileHeader) + sizeof(BMPinfoHeader);

            new_info_header = cur_info_header;
            new_info_header.biBitCount = 24;
            new_info_header.biClrUsed = 0;
*/
            /* Rotate picture */
            palette_rgb tempPixel;
            for (i = 0; i < (image_dots / 2); i++) {
                tempPixel = *bmpImgStrt;
                *bmpImgStrt = *bmpImgEnd;
                *bmpImgEnd = tempPixel;

                *bmpImgStrt++;
                *bmpImgEnd--;
            }

/*
            int * new_file_adr;
            new_file_adr = (int *) (&new_file_header);
            write(fd_out, new_file_adr, sizeof (BMPfileHeader));

            new_file_adr = (int *) (&new_info_header);
            write(fd_out, new_file_adr, sizeof (BMPinfoHeader));

            write(fd_out, new_bitmap_image_pointer_start, (image_dots * sizeof(palette_rgb)));
            close(fd_out);
*/
            /* Clearing memory */
            new_bitmap_image_pointer = new_bitmap_image_pointer_start;
            bitmap_image_pointer = bitmap_image_pointer_start;
            //free(new_bitmap_image_pointer);
            free(bitmap_image_pointer);
            close(fd);
            return new_bitmap_image_pointer;
        }

        else if (16 == img_bitCount) {
            if (BI_RGB == cur_info_header.biCompression) {
                /* Read current image martix */
                if (32768 == cur_info_header.biClrUsed) {
                    /* Read current image martix */
                    palette_rgb_5_5_5 *bitmap_image_pointer_start;
                    palette_rgb_5_5_5 *bitmap_image_pointer = malloc(image_dots * sizeof(palette_rgb_5_5_5));
                    /* Backup pointer to the start of image for free memory in future */
                    bitmap_image_pointer_start = bitmap_image_pointer;

                    lseek(fd, (cur_file_header.bfOffBits), FROM_START);
                    read(fd, bitmap_image_pointer, (image_dots * sizeof(palette_rgb_5_5_5)) );

                    printf("Read_OK\n");

                    /* Converting  to 24-bit image */
                    palette_rgb *new_bitmap_image_pointer = malloc(image_dots * sizeof(palette_rgb));
                    palette_rgb *new_bitmap_image_pointer_start;
                    /* Backup pointer to the start of image for free memory in future */
                    new_bitmap_image_pointer_start = new_bitmap_image_pointer;

                    dword_t i;
                    palette_rgb newpixel;
                    for (i = 0; i < image_dots; i++) {
                        newpixel.blue  = (*(bitmap_image_pointer )).blue;
                        newpixel.green = (*(bitmap_image_pointer )).green;
                        newpixel.red   = (*(bitmap_image_pointer )).red;

                        *new_bitmap_image_pointer = newpixel;
                        *new_bitmap_image_pointer++;
                        *bitmap_image_pointer++;
                    }

                    /* Clearing memory */
                    new_bitmap_image_pointer = new_bitmap_image_pointer_start;
                    bitmap_image_pointer = bitmap_image_pointer_start;
                    //free(new_bitmap_image_pointer);
                    free(bitmap_image_pointer);
                    close(fd);
                    return new_bitmap_image_pointer;
                }
                else if (65536 == cur_info_header.biClrUsed) {
                    /* Read current image martix */
                    palette_rgb_5_6_5 *bitmap_image_pointer_start;
                    palette_rgb_5_6_5 *bitmap_image_pointer = malloc(image_dots * sizeof(palette_rgb_5_6_5));
                    /* Backup pointer to the start of image for free memory in future */
                    bitmap_image_pointer_start = bitmap_image_pointer;

                    lseek(fd, (cur_file_header.bfOffBits), FROM_START);
                    read(fd, bitmap_image_pointer, (image_dots * sizeof(palette_rgb_5_6_5)) );

                    printf("Read_OK\n");

                    /* Converting  to 24-bit image */
                    palette_rgb *new_bitmap_image_pointer = malloc(image_dots * sizeof(palette_rgb));
                    palette_rgb *new_bitmap_image_pointer_start;
                    /* Backup pointer to the start of image for free memory in future */
                    new_bitmap_image_pointer_start = new_bitmap_image_pointer;

                    dword_t i;
                    palette_rgb newpixel;
                    for (i = 0; i < image_dots; i++) {
                        newpixel.blue  = (*(bitmap_image_pointer )).blue;
                        newpixel.green = (*(bitmap_image_pointer )).green;
                        newpixel.red   = (*(bitmap_image_pointer )).red;

                        *new_bitmap_image_pointer = newpixel;
                        *new_bitmap_image_pointer++;
                        *bitmap_image_pointer++;
                    }

                    /* Clearing memory */
                    new_bitmap_image_pointer = new_bitmap_image_pointer_start;
                    bitmap_image_pointer = bitmap_image_pointer_start;
                    //free(new_bitmap_image_pointer);
                    free(bitmap_image_pointer);
                    close(fd);
                    return new_bitmap_image_pointer;
                }
                else {
                    printf("Unsupported BMP-file format\n");
                }
            }
            else if (BI_BITFIELDS == cur_info_header.biCompression) {
                lseek(fd, (sizeof(BMPfileHeader) + cur_info_header.biSize), FROM_START);    /* lseek to the palette */
                palette_biCompression palette_mask;

                cur_file_adr = (int *) (&palette_mask);
                read(fd, cur_file_adr, sizeof(palette_mask) );   /* read color palette_mask */

                /* Read current image martix */
                word_t *bitmap_image_pointer_start;
                word_t *bitmap_image_pointer = malloc(image_dots * sizeof(word_t));
                /* Backup pointer to the start of image for free memory in future */
                bitmap_image_pointer_start = bitmap_image_pointer;

                lseek(fd, (cur_file_header.bfOffBits), FROM_START);
                read(fd, bitmap_image_pointer, (image_dots * sizeof(word_t)) );

                printf("Read_OK\n");

                /* Converting  to 24-bit image */
                palette_rgb *new_bitmap_image_pointer = malloc(image_dots * sizeof(palette_rgb));
                palette_rgb *new_bitmap_image_pointer_start;
                /* Backup pointer to the start of image for free memory in future */
                new_bitmap_image_pointer_start = new_bitmap_image_pointer;

                dword_t i;
                palette_rgb newpixel;
                for (i = 0; i < image_dots; i++) {
                    newpixel.blue  = (*(bitmap_image_pointer )) & palette_mask.blue_mask;
                    newpixel.green = (*(bitmap_image_pointer )) & palette_mask.green_mask;
                    newpixel.red   = (*(bitmap_image_pointer )) & palette_mask.red_mask;

                    *new_bitmap_image_pointer = newpixel;
                    *new_bitmap_image_pointer++;
                    *bitmap_image_pointer++;
                }

                /* Clearing memory */
                new_bitmap_image_pointer = new_bitmap_image_pointer_start;
                bitmap_image_pointer = bitmap_image_pointer_start;
                //free(new_bitmap_image_pointer);
                free(bitmap_image_pointer);
                close(fd);
                return new_bitmap_image_pointer;
            }
        }

        else if (24 == img_bitCount) {
            /* Read current image martix */
            palette_rgb *bitmap_image_pointer = malloc(image_dots * sizeof(palette_rgb));

            lseek(fd, (cur_file_header.bfOffBits), FROM_START);
            read(fd, bitmap_image_pointer, image_dots * sizeof(palette_rgb));
            printf("Read_OK\n");

            /* Clearing memory */
            //free(bitmap_image_pointer);
            close(fd);
            return bitmap_image_pointer;
        }

        else if (32 == img_bitCount) {
            if (BI_RGB == cur_info_header.biCompression) {

                /* Read current image martix */
                palette_8_bit *bitmap_image_pointer_start;
                palette_8_bit *bitmap_image_pointer = malloc(image_dots * sizeof(palette_8_bit));
                bitmap_image_pointer_start = bitmap_image_pointer;  /* Backup pointer to the start of image for free memory in future */

                lseek(fd, (cur_file_header.bfOffBits), FROM_START);
                read(fd, bitmap_image_pointer, (image_dots * sizeof(palette_8_bit)) );

                printf("Read_OK\n");

                /* Converting  to 24-bit image */
                palette_rgb *new_bitmap_image_pointer = malloc(image_dots * sizeof(palette_rgb));
                palette_rgb *new_bitmap_image_pointer_start;
                new_bitmap_image_pointer_start = new_bitmap_image_pointer;  /* Backup pointer to the start of image for free memory in future */
                dword_t i;
                palette_rgb newpixel;
                for (i = 0; i < image_dots; i++) {
                    newpixel.blue  = (*(bitmap_image_pointer )).blue;
                    newpixel.green = (*(bitmap_image_pointer )).green;
                    newpixel.red   = (*(bitmap_image_pointer )).red;

                    *new_bitmap_image_pointer = newpixel;
                    *new_bitmap_image_pointer++;
                    *bitmap_image_pointer++;
                }

                /* Clearing memory */
                new_bitmap_image_pointer = new_bitmap_image_pointer_start;
                bitmap_image_pointer = bitmap_image_pointer_start;
                //free(new_bitmap_image_pointer);
                free(bitmap_image_pointer);
                close(fd);
                return new_bitmap_image_pointer;
            }
        }

        else {
            printf("Unsupported BMP-file format");
            return NULL;
        }
    close(fd);
    return NULL;
}

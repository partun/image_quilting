#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
#include "load_image.h"

void free_image(Image *img) {
    free(img->data);
    free(img);
}

Image *read_image(char *image_path) {
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    FILE *infile;
    JSAMPARRAY pJpegBuffer;
    if ((infile = fopen(image_path, "rb")) == NULL) {
        fprintf(stderr, "can't open image %s\n", image_path);
        exit(1);
    }
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);
    (void) jpeg_read_header(&cinfo, TRUE);
    (void) jpeg_start_decompress(&cinfo);
    int width = cinfo.output_width;
    int height = cinfo.output_height;

    printf("width: %d height: %d\n", width, height);

    RGB *image_data = malloc(width * height * sizeof(RGB));
    if (!image_data) {
        printf("NO MEM FOR JPEG CONVERT!\n");
        return 0;
    }
    int row_stride = width * cinfo.output_components;
    pJpegBuffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

    unsigned char r, g, b;
    int i = 0;
    while (cinfo.output_scanline < cinfo.output_height) {
        (void) jpeg_read_scanlines(&cinfo, pJpegBuffer, 1);
        for (int x = 0; x < width; x++) {
            r = pJpegBuffer[0][cinfo.output_components * x];
            if (cinfo.output_components > 2) {
                g = pJpegBuffer[0][cinfo.output_components * x + 1];
                b = pJpegBuffer[0][cinfo.output_components * x + 2];
            } else {
                g = r;
                b = r;
            }
            image_data[i].b = b;
            image_data[i].g = g;
            image_data[i].r = r;
            i++;
        }
    }
    fclose(infile);
    (void) jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    Image *image = malloc(sizeof(Image));
    image->data = image_data;
    image->width = width;
    image->height = height;
    return image;
}

void store_image(Image *img, char *image_path) {
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    cinfo.image_width = img->width;
    cinfo.image_height = img->height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, 100, TRUE);

    // open output file
    FILE *outfile;
    if ((outfile = fopen(image_path, "wb")) == NULL) {
        fprintf(stderr, "Failed to open output file\n");
        exit(1);
    }
    jpeg_stdio_dest(&cinfo, outfile);

    jpeg_start_compress(&cinfo, TRUE);
    // Write image data to JPEG compressor in scanline format
    JSAMPROW row_pointer[1];
    unsigned char *image_buffer = (unsigned char *) img->data;
    int row_stride = img->width * 3;
    while (cinfo.next_scanline < cinfo.image_height) {
        row_pointer[0] = &image_buffer[cinfo.next_scanline * row_stride];
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    // free all resources
    jpeg_finish_compress(&cinfo);
    fclose(outfile);
    free_image(img);
}


void print_image_matrix(Image *img, Color color) {
    int width = img->width;
    int height = img->height;

    switch (color) {
        case RED:
            printf("RED:\n");
            break;
        case GREEN:
            printf("GREEN:\n");
            break;
        case BLUE:
            printf("BLUE:\n");
            break;
    }

    for (int i = 0; i < height; ++i) {
        printf("%2d:", i);
        for (int j = 0; j < width; ++j) {
            RGB rgb = img->data[i * width + j];
            switch (color) {
                case RED:
                    printf(" %3d ", rgb.r);
                    break;
                case GREEN:
                    printf(" %3d ", rgb.g);
                    break;
                case BLUE:
                    printf(" %3d ", rgb.b);
                    break;
            }
        }
        printf("\n");
    }
}
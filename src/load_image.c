#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
#include "load_image.h"
#include "image.h"


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

ImageRGB *read_image_rgb(char *image_path) {
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


    ImageRGB *image = malloc(sizeof(ImageRGB));
    image->r_data = malloc(width * height * sizeof(ColorV));
    image->g_data = malloc(width * height * sizeof(ColorV));
    image->b_data = malloc(width * height * sizeof(ColorV));
    image->width = width;
    image->height = height;


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
            image->b_data[i] = b;
            image->g_data[i] = g;
            image->r_data[i] = r;
            i++;
        }
    }
    fclose(infile);
    (void) jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

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
//    free_image(img);
}

void store_image_rgb(ImageRGB *img, char *image_path) {
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


    RGB *tmp_image_buffer = (RGB *) malloc(img->width * img->height * sizeof(RGB));
    for (int i = 0; i < img->width * img->height; ++i) {
        tmp_image_buffer[i].r = img->r_data[i];
        tmp_image_buffer[i].g = img->g_data[i];
        tmp_image_buffer[i].b = img->b_data[i];
    }
    unsigned char *image_buffer = (unsigned char *) tmp_image_buffer;
    
    int row_stride = img->width * 3;
    while (cinfo.next_scanline < cinfo.image_height) {
        row_pointer[0] = &image_buffer[cinfo.next_scanline * row_stride];
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    // free all resources
    jpeg_finish_compress(&cinfo);
    fclose(outfile);
//    free_image(img);
}











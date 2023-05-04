#include "image.h"
#include <stdlib.h>
#include <stdio.h>

void free_image(Image *img) {
    free(img->data);
    free(img);
}

void print_image_matrix(Image *img, Color color) {
    unsigned int width = img->width;
    unsigned int height = img->height;

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

int rgb_equal(RGB *a, RGB *b) {
    return a->r == b->r && a->g == b->g && a->b == b->b;
}

int rgb_sq_error(RGB *a, RGB *b) {
    int r_error = a->r - b->r;
    int g_error = a->g - b->g;
    int b_error = a->b - b->b;

    r_error = r_error * r_error;
    g_error = g_error * g_error;
    b_error = b_error * b_error;
    return r_error + g_error + b_error;
}

int image_equal(Image *a, Image *b) {
    if (a->width != b->width || a->height != b->height) {
        return 0;
    }
    int height = a->height;
    int width = b->width;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (!rgb_equal(a->data + (i * width + j), b->data + (i * width + j))) {
                return 0;
            }
        }
    }
    return 1;
}

/*
 * create a solid color image
 */
Image *make_solid_image(unsigned int width, unsigned int height, RGB c) {
    RGB *data = (RGB *) malloc(width * height * sizeof(RGB));
    Image *img = (Image *) malloc(sizeof(Image));
    img->data = data;
    img->width = width;
    img->height = height;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            img->data[i * width + j].r = c.r;
            img->data[i * width + j].g = c.g;
            img->data[i * width + j].b = c.b;
        }
    }
    return img;
}
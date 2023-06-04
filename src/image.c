#include "image.h"
#include <stdlib.h>
#include <stdio.h>

void free_image(Image *img) {
    free(img->data);
    free(img);
}

void free_image_rgb(ImageRGB *img) {
    free(img->r_data);
    free(img->g_data);
    free(img->b_data);
    free(img);
}

void permute_image_colors(ImageRGB *im, ColorOrder order) {
    unsigned char *im_data_r = im->r_data;
    unsigned char *im_data_g = im->g_data;
    unsigned char *im_data_b = im->b_data;

    switch (order) {
        case GRB_ORDER:
            im->r_data = im_data_g;
            im->g_data = im_data_r;
            im->b_data = im_data_b;
            break;
        case GBR_ORDER:
            im->r_data = im_data_g;
            im->g_data = im_data_b;
            im->b_data = im_data_r;
            break;
        case RGB_ORDER:
            break;
        case RBG_ORDER:
            im->r_data = im_data_r;
            im->g_data = im_data_b;
            im->b_data = im_data_g;
            break;
        case BRG_ORDER:
            im->r_data = im_data_b;
            im->g_data = im_data_r;
            im->b_data = im_data_g;
            break;

        case BGR_ORDER:
            im->r_data = im_data_b;
            im->g_data = im_data_g;
            im->b_data = im_data_r;
            break;
    }
}

void print_image_order(ColorOrder order) {
    switch (order) {
        case GRB_ORDER:
            printf("GRB order\n");
            break;
        case GBR_ORDER:
            printf("GBR order\n");
            break;
        case RGB_ORDER:
            printf("RGB order\n");
            break;
        case RBG_ORDER:
            printf("RBG order\n");
            break;
        case BRG_ORDER:
            printf("BRG order\n");
            break;
        case BGR_ORDER:
            printf("BGR order\n");
            break;
    }
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
Image *make_solid_image(int width, int height, RGB c) {
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

ImageRGB *convert_image_to_image_rgb(Image *img) {
    ImageRGB *out = (ImageRGB *) malloc(sizeof(ImageRGB));
    out->width = img->width;
    out->height = img->width;
    out->r_data = malloc(out->width * out->height * sizeof(ColorV));
    out->g_data = malloc(out->width * out->height * sizeof(ColorV));
    out->b_data = malloc(out->width * out->height * sizeof(ColorV));


    for (int i = 0; i < img->width * img->height; ++i) {
        out->r_data[i] = img->data[i].r;
        out->g_data[i] = img->data[i].g;
        out->b_data[i] = img->data[i].b;
    }

    return out;
}

//
// Created by Dominic Steiner on 28.02.23.
//

#ifndef TEAM08_LOAD_IMAGE_H
#define TEAM08_LOAD_IMAGE_H

typedef enum {
    RED,
    GREEN,
    BLUE,
} Color;

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} RGB;

typedef struct {
    int width;
    int height;
    RGB *data;
} Image;

Image *read_image(char *image_path);

void store_image(Image *img, char *image_path);

void print_image_matrix(Image *img, Color color);

#endif //TEAM08_LOAD_IMAGE_H

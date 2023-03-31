#ifndef TEAM08_LOAD_IMAGE_H
#define TEAM08_LOAD_IMAGE_H

typedef enum Color {
    RED,
    GREEN,
    BLUE,
} Color;

typedef struct RGB {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} RGB;

typedef struct Image {
    unsigned int width;
    unsigned int height;
    RGB *data;
} Image;

typedef struct Matrix {
    unsigned int width;
    unsigned int height;
    int *data;
} Matrix;

void free_image(Image *img);

void free_matrix(Matrix *m);

Image *read_image(char *image_path);

void store_image(Image *img, char *image_path);

void print_image_matrix(Image *img, Color color);

void print_matrix(Matrix *m);

int rgb_sq_error(RGB *a, RGB *b);

int rgb_equal(RGB *a, RGB *b);

int image_equal(Image *a, Image *b);

int matrix_equal(Matrix *a, Matrix *b);

Image *make_solid_image(unsigned int width, unsigned int height, RGB color);

#endif //TEAM08_LOAD_IMAGE_H

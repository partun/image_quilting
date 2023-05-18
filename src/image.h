#ifndef TEAM08_IMAGE_H
#define TEAM08_IMAGE_H

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
    int width;
    int height;
    RGB *data;
} Image;

typedef struct ImageCoordinates {
    int x;
    int y;
} ImageCoordinates;

void free_image(Image *img);

void print_image_matrix(Image *img, Color color);

int rgb_sq_error(RGB *a, RGB *b);

int rgb_equal(RGB *a, RGB *b);

int image_equal(Image *a, Image *b);

Image *make_solid_image(unsigned int width, unsigned int height, RGB color);


#endif //TEAM08_IMAGE_H

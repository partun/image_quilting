#ifndef TEAM08_IMAGE_H
#define TEAM08_IMAGE_H

typedef enum Color {
    RED,
    GREEN,
    BLUE,
} Color;

typedef enum ColorOrder {
    RGB_ORDER,
    RBG_ORDER,
    GRB_ORDER,
    GBR_ORDER,
    BRG_ORDER,
    BGR_ORDER,
} ColorOrder;


typedef unsigned char ColorV;

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

typedef struct ImageRGB {
    int width;
    int height;
    ColorV *r_data;
    ColorV *g_data;
    ColorV *b_data;
} ImageRGB;

typedef struct ImageCoordinates {
    int x;
    int y;
} ImageCoordinates;

void print_image_order(ColorOrder order);

void free_image(Image *img);

void free_image_rgb(ImageRGB *img);

void print_image_matrix(Image *img, Color color);

int rgb_sq_error(RGB *a, RGB *b);

int rgb_equal(RGB *a, RGB *b);

int image_equal(Image *a, Image *b);

Image *make_solid_image(int width, int height, RGB color);

ImageRGB *convert_image_to_image_rgb(Image *img);

void permute_image_colors(ImageRGB *im, ColorOrder order);

#endif //TEAM08_IMAGE_H

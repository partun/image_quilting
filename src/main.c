#include <stdio.h>
#include "load_image.h"


void invert_image(Image *img) {
    int width = img->width;
    int height = img->height;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            RGB *rgb = (int *) &img->data[i * width + j];
            rgb->r = rgb->r ^ 0xFF;
            rgb->g = rgb->g ^ 0xFF;
            rgb->b = rgb->b ^ 0xFF;
        }
    }
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("provide a image path as argument.\n");
        return 1;
    }

    char *image_path = argv[1];
    Image *img = read_image(image_path);

    // print_image_matrix(img, GREEN);
    // print_image_matrix(img, BLUE);
    // print_image_matrix(img, RED);

    invert_image(img);

    store_image(img, "output/test.jpeg");
}

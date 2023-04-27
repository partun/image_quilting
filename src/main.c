#include <stdio.h>
#include "load_image.h"
#include "quilting.h"


void invert_image(Image *img) {
    unsigned int width = img->width;
    unsigned int height = img->height;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            RGB *rgb = (RGB *) &img->data[i * width + j];
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

    //print_image_matrix(img, GREEN);
    //print_image_matrix(img, BLUE);
    //print_image_matrix(img, RED);

/*    invert_image(img);
    store_image(img, "output/tests.jpeg");
*/
    Image *quilt = quilting(img, 20, 14, 3);
    store_image(quilt, "output/quilt.jpeg");

//    print_image_matrix(quilt, GREEN);
//    print_image_matrix(quilt, BLUE);
//    print_image_matrix(quilt, RED);

    printf("completed quilting\n");
}

#include <stdio.h>
#include "load_image.h"


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("provide a image path as argument.\n");
        return 1;
    }

    char *image_path = argv[1];
    Image *img = read_image(image_path);

    print_image_matrix(img, RED);
    print_image_matrix(img, GREEN);
    print_image_matrix(img, BLUE);
}

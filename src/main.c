#include <stdio.h>
#include "load_image.h"
#include "quilting.h"
#include "timing.h"

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

/**
 * @param argv[1] path to input image
 * @param argv[2] (optional) path to output text file for measurement
 */
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("provide a image path as argument.\n");
        return 1;
    } else if (argc < 3) {
        char *image_path = argv[1];
        Image *img = read_image(image_path);

        Image *quilt = quilting(img, 32, 8, 8);
        store_image(quilt, "output/quilt.jpeg");

        return (0);
    } else {

        char *image_path = argv[1];
        Image *img = read_image(image_path);

        FILE *file = fopen(argv[2], "w");
        if (file == NULL) {
            printf("Error opening file %s\n", argv[2]);
            return (1);
        }

//        store_image(img, "output/test2.jpeg");

//    time_quilt((quilting), img, 10, 14, 3);
        multi_time_quilt((quilting), file, img, 8, 16, 2, 10, 16, 1, 1, 5, 1);

        fclose(file);

    }
    printf("completed quilting\n");
}



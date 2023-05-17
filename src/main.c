#include <stdio.h>
#include "load_image.h"
#include "quilting_opt.h"
#include "quilting.h"
#include "quilting_opt.h"
#include "timing.h"


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

   //     Image *quilt = quilting_opt(img, 32, 8, 8);
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

//        time_quilt((quilting), img, 10, 14, 3);
        multi_time_quilt((quilting), file, img,
                         16, 17, 2, 10,
                         11, 2, 4, 9, 2);

        free_image(img);
        fclose(file);

    }
    printf("completed quilting\n");
}



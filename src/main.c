#include <stdio.h>
#include "load_image.h"
#include "src/baseline/quilting.h"
#include "src/opt_1/quilting_opt.h"
#include "src/opt_2/quilting_opt_2.h"
#include "src/opt_3/quilting_opt_3.h"
#include "src/opt_4/quilting_opt_4.h"
#include "src/opt_5/quilting_opt_5.h"
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
        ImageRGB *img_rgb = read_image_rgb(image_path);
        Image *img = read_image(image_path);

        ImageRGB *quilt_rgb = quilting_opt_5(img_rgb, 64, 12, 24);
        Image *quilt = quilting_baseline(img, 64, 12, 24);

        store_image_rgb(quilt_rgb, "output/quilt_rgb.jpeg");
        store_image(quilt, "output/quilt.jpeg");

        return (0);
    } else {

        char *image_path = argv[1];
        Image *img = read_image(image_path);
        ImageRGB *img_rgb = read_image_rgb(image_path);

        FILE *file = fopen(argv[2], "w");
        if (file == NULL) {
            printf("Error opening file %s\n", argv[2]);
            return (1);
        }

//        time_quilt((quilting), img, 10, 14, 3);
        printf("opt_5");
        multi_time_quilt_rgb(
                (quilting_opt_5), file, img_rgb,
                64, 65, 1,
                12, 13, 1,
                24, 25, 1
        );
        printf("opt_4");
        multi_time_quilt(
                (quilting_opt_4), file, img,
                64, 65, 1,
                12, 13, 1,
                24, 25, 1
        );
        printf("opt_3");
        multi_time_quilt(
                (quilting_opt_3), file, img,
                64, 65, 1,
                12, 13, 1,
                24, 25, 1
        );
//        printf("opt_2");
//        multi_time_quilt(
//                (quilting_opt_2), file, img,
//                64, 65, 1,
//                12, 13, 1,
//                24, 25, 1
//        );
//        printf("opt_1");
//        multi_time_quilt(
//                (quilting_opt), file, img,
//                64, 65, 1,
//                12, 13, 1,
//                24, 25, 1
//        );
//        printf("baseline");
//        multi_time_quilt(
//                (quilting_baseline), file, img,
//                64, 65, 1,
//                12, 13, 1,
//                24, 25, 1
//        );


        free_image(img);
        fclose(file);

    }
    printf("completed quilting\n");
}



#include <stdio.h>
#include "load_image.h"
#include "src/baseline/quilting.h"
#include "src/opt_1/quilting_opt.h"
#include "src/opt_2/quilting_opt_2.h"
#include "src/opt_3/quilting_opt_3.h"
#include "src/opt_4/quilting_opt_4.h"
#include "src/opt_5/quilting_opt_5.h"
#include "src/opt_6/quilting_opt_6.h"
#include "src/opt_6a/quilting_opt_6a.h"
#include "src/opt_6b/quilting_opt_6b.h"
#include "src/opt_6c/quilting_opt_6c.h"
#include "src/opt_6d/quilting_opt_6d.h"
#include "src/opt_7/quilting_opt_7.h"
#include "src/opt_8/quilting_opt_8.h"
#include "src/opt_9/quilting_opt_9.h"
#include "src/opt_10/quilting_opt_10.h"
#include "timing.h"


/**
 * @param argv[1] path to input image
 * @param argv[2] (optional) path to output text file for measurement
 */
int main(int argc, char *argv[]) {
    if (argc < 2) {
        time_total();
        return 0;
    } else if (argc < 3) {
        char *image_path = argv[1];
        ImageRGB *img_rgb = read_image_rgb(image_path);
//        Image *img = read_image(image_path);

        ImageRGB *quilt_rgb = quilting_opt_10(img_rgb, 64, 32, 24);
//        Image *quilt = quilting_baseline(img, 64, 12, 24);

        store_image_rgb(quilt_rgb, "output/quilt_opt.jpeg");
//        store_image(quilt, "output/quilt_baseline.jpeg");

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
//        printf("opt_8");
//        multi_time_quilt_rgb(
//                (quilting_opt_8), file, img_rgb,
//                64, 65, 1,
//                12, 13, 1,
//                32, 33, 1
//        );1
//        printf("opt_7");
//        multi_time_quilt_rgb(
//                (quilting_opt_7), file, img_rgb,
//                64, 65, 1,
//                12, 13, 1,
//                24, 25, 1
//        );
        printf("opt_10");
        multi_time_quilt_rgb(
                (quilting_opt_10), file, img_rgb,
                64, 65, 1,
                12, 13, 1,
                32, 33, 1
        );
        printf("opt_9");
        multi_time_quilt_rgb(
                (quilting_opt_9), file, img_rgb,
                64, 65, 1,
                12, 13, 1,
                32, 33, 1
        );
//        printf("opt_6d");
//        multi_time_quilt_rgb(
//                (quilting_opt_6d), file, img_rgb,
//                64, 65, 1,
//                12, 13, 1,
//                32, 33, 1
//        );
        printf("opt_6c");
        multi_time_quilt_rgb(
                (quilting_opt_6c), file, img_rgb,
                64, 65, 1,
                12, 13, 1,
                32, 33, 1
        );
//        printf("opt_6b");
//        multi_time_quilt_rgb(
//                (quilting_opt_6b), file, img_rgb,
//                64, 65, 1,
//                12, 13, 1,
//                32, 33, 1
//        );
//        printf("opt_6a");
//        multi_time_quilt_rgb(
//                (quilting_opt_6a), file, img_rgb,
//                64, 65, 1,
//                12, 13 1,
//                32, 33, 1
//        );
//        printf("opt_6");
//        multi_time_quilt_rgb(
//                (quilting_opt_6), file, img_rgb,
//                64, 65, 1,
//                12, 13, 1,
//                32, 33, 1
//        );
//        printf("opt_5");
//        multi_time_quilt_rgb(
//                (quilting_opt_5), file, img_rgb,
//                64, 65, 1,
//                12, 13, 1,
//                24, 25, 1
//        );
//        printf("opt_4");
//        multi_time_quilt(
//                (quilting_opt_4), file, img,
//                64, 65, 1,
//                12, 13, 1,
//                24, 25, 1
//        );
//        printf("opt_3");
//        multi_time_quilt(
//                (quilting_opt_3), file, img,
//                64, 65, 1,
//                12, 13, 1,
//               24, 25, 1
//        );
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



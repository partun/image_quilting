//
//
//

#ifndef CALC_ERRORS_OPT_10_H
#define CALC_ERRORS_OPT_10_H

#include "src/load_image.h"
#include "src/matrix.h"

extern int min_err;

ColorOrder get_image_color_order(ImageRGB *im);

void calc_errors_opt_10(
        int *errors,
        ImageRGB *src, ImageRGB *out, ImageCoordinates out_coord, int block_size,
        int overlap, Direction direction
);

ImageCoordinates find_best_block_opt_10(
        int *errors, ImageRGB *src, int block_size
);

#endif //CALC_ERRORS_OPT_10_H

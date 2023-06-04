//
//
//

#ifndef CALC_ERRORS_OPT_11_H
#define CALC_ERRORS_OPT_11_H

#include "src/load_image.h"
#include "src/matrix.h"

extern int min_err;

ColorOrder get_image_color_order_opt_11(ImageRGB *im);

void calc_errors_opt_11(
        int *errors,
        ImageRGB *src, ImageRGB *out, ImageCoordinates out_coord, int block_size,
        int overlap, Direction direction, ImageCoordinates im_coords_top,
        ImageCoordinates im_coords_left
);

ImageCoordinates find_best_block_opt_11(
        int *errors, ImageRGB *src, int block_size
);

#endif //CALC_ERRORS_OPT_11_H

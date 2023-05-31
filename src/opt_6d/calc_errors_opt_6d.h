//
//
//

#ifndef CALC_ERRORS_OPT_6d_H
#define CALC_ERRORS_OPT_6d_H

#include "src/load_image.h"
#include "src/matrix.h"

extern int min_err;

void calc_errors_opt_6d(
        int *errors,
        ImageRGB *src, ImageRGB *out, ImageCoordinates out_coord, int block_size,
        int overlap, Direction direction
);

ImageCoordinates find_best_block_opt_6d(
        const int *errors, ImageRGB *src, int block_size,
        ImageCoordinates *candidate_coords
);

#endif //CALC_ERRORS_OPT_6d_H

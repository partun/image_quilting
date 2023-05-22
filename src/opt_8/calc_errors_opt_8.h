//
//
//

#ifndef CALC_ERRORS_OPT_8_H
#define CALC_ERRORS_OPT_8_H

#include "src/load_image.h"
#include "src/matrix.h"

extern int min_err;

void calc_errors_opt_8(
        int *errors,
        ImageRGB *src, ImageRGB *out, ImageCoordinates out_coord, int block_size,
        int overlap, Direction direction
);

ImageCoordinates find_best_block_opt_8(
        const int *errors, ImageRGB *src, int block_size,
        ImageCoordinates *candidate_coords
);

#endif //CALC_ERRORS_OPT_8_H

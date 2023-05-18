#ifndef CALC_ERRORS_OPT_2_H
#define CALC_ERRORS_OPT_2_H

#include "src/load_image.h"
#include "src/matrix.h"

int *calc_errors_opt_2(
        Image *src, Image *out, ImageCoordinates out_coord, int block_size,
        int overlap, Direction direction
);

ImageCoordinates find_best_block_opt_2(const int *errors, Image *src, int block_size);

#endif //CALC_ERRORS_OPT_2_H

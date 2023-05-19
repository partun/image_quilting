//
// Created by Oleh Kuzyk on 10.03.23.
//

#ifndef CALC_ERRORS_OPT_3_H
#define CALC_ERRORS_OPT_3_H

#include "src/load_image.h"
#include "src/matrix.h"

void calc_errors_opt_3(
        int *errors,
        Image *src, Image *out, ImageCoordinates out_coord, int block_size,
        int overlap, Direction direction
);

ImageCoordinates find_best_block_opt_3(const int *errors, Image *src, int block_size);

#endif //CALC_ERRORS_OPT_3_H

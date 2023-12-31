//
// Created by Oleh Kuzyk on 10.03.23.
//

#ifndef CALC_ERRORS_H
#define CALC_ERRORS_H

#include "src/load_image.h"
#include "src/matrix.h"

double *calc_errors(
        Image *src, Image *out, ImageCoordinates out_coord, int block_size,
        int overlap, Direction direction
);

ImageCoordinates find_best_block(const double *errors, Image *src, int block_size);

#endif //CALC_ERRORS_H

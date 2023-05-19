//
//
//

#ifndef CALC_ERRORS_OPT_4_H
#define CALC_ERRORS_OPT_4_H

#include "src/load_image.h"
#include "src/matrix.h"

extern int min_err;

void calc_errors_opt_4(
        int *errors,
        Image *src, Image *out, ImageCoordinates out_coord, int block_size,
        int overlap, Direction direction
);

ImageCoordinates find_best_block_opt_4(const int *errors, Image *src, int block_size,
                                       ImageCoordinates *candidate_coords);

#endif //CALC_ERRORS_OPT_4_H

#ifndef TEAM08_MIN_CUT_OPT_2_H
#define TEAM08_MIN_CUT_OPT_2_H

#include "src/matrix.h"
#include "src/image.h"


Matrix *calc_overlap_error_opt_2(
        Image *source_image, Image *output_image, ImageCoordinates block_coords,
        ImageCoordinates output_coords,
        int overlap_width, int overlap_height
);


Matrix *min_cut_opt_2(
        Image *source_image, Image *output_image, ImageCoordinates block_coords,
        ImageCoordinates output_coords,
        int block_size, int overlap, Direction direction
);

#endif //TEAM08_MIN_CUT_OPT_2_H

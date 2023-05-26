#ifndef TEAM08_MIN_CUT_OPT_6b_H
#define TEAM08_MIN_CUT_OPT_6b_H

#include "src/matrix.h"
#include "src/image.h"


Matrix *calc_overlap_error_opt_6b(
        ImageRGB *source_image, ImageRGB *output_image, ImageCoordinates block_coords,
        ImageCoordinates output_coords,
        int overlap_width, int overlap_height
);


CutMatrix *min_cut_opt_6b(
        ImageRGB *source_image, ImageRGB *output_image, ImageCoordinates block_coords,
        ImageCoordinates output_coords,
        int block_size, int overlap, Direction direction
);

#endif //TEAM08_MIN_CUT_OPT_6b_H

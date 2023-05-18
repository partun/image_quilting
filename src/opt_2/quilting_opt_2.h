#ifndef QUILTING_OPT_2_H
#define QUILTING_OPT_2_H

#include "src/load_image.h"
#include "src/opt_2/calc_errors_opt_2.h"

Image *quilting_opt_2(Image *image, int block_size, int out_num_blocks, int overlap_size);

#endif //QUILTING_OPT_2_H

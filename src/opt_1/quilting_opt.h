#ifndef QUILTING_OPT_H
#define QUILTING_OPT_H

#include "src/load_image.h"
#include "src/opt_1/calc_errors_opt.h"

Image *quilting_opt(Image *image, int block_size, int out_num_blocks, int overlap_size);

#endif //QUILTING_OPT_H

#include "load_image.h"
#include "calc_errors.h"
#include "quilting.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <float.h>
#include <time.h>
#include <math.h>

#define RSEED 8

void find_indexes(double *errors, int *inds,
                  int num_blocks_h, int num_blocks_w) {

    double min_err = DBL_MAX;
    inds[0] = -1;
    inds[1] = -1;
    //printf("errors: %d, %d\n",num_blocks_h, num_blocks_w);
    for (int y = 0; y < num_blocks_h; y++) {
        for (int x = 0; x < num_blocks_w; x++) {
            double err = errors[y * num_blocks_w + x];
            //      printf("%.1f ", err);
            if (err < min_err) {
                min_err = err;
                inds[0] = y;
                inds[1] = x;
            }
        }
        //   printf("\n");
    }
    //printf("index - y:%d x:%d\n", inds[0], inds[1]);
    if (inds[0] < 0 || inds[1] < 0) {
        fprintf(stderr, "error: find index negative: x=%d y=%d", inds[1], inds[0]);
        exit(1);
    }
    return;
}

void copy_blocks(RGB *im_data, RGB *out_data,
                 int im_width, int out_width,
                 int over_x, int over_y, int bx, int by,
                 int out_si, int out_sj, int im_si, int im_sj) {

    for (int i = over_y; i < by; i++) {
        for (int j = over_x; j < bx; j++) {
            out_data[(out_si + i) * out_width + out_sj + j] =
                    im_data[(im_si + i) * im_width + im_sj + j];
        }
    }
    return;
}


void
copy_section(Image *source_image, Image *output_image, ImageCoordinates source_coords, ImageCoordinates output_coords,
             int width, int height) {
    unsigned int source_idx;
    unsigned int output_idx;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            source_idx = (source_coords.y + y) * source_image->width + (source_coords.x + x);
            output_idx = (output_coords.y + y) * output_image->width + (output_coords.x + x);
            output_image->data[output_idx] = source_image->data[source_idx];
        }
    }
}

/*
 * calculates the error in the overlapping section
 */
Matrix *calc_overlap_error(Image *source_image, Image *output_image, ImageCoordinates block_coords,
                           ImageCoordinates output_coords,
                           int overlap_width, int overlap_height) {

    int *overlap_error = (int *) malloc(overlap_width * overlap_height * sizeof(int));

    int source_idx;
    int output_idx;
    int error_idx;
    for (int y = 0; y < overlap_height; y++) {
        for (int x = 0; x < overlap_width; x++) {
            error_idx = y * overlap_width + x;
            source_idx = (block_coords.y + y) * source_image->width + (block_coords.x + x);
            output_idx = (output_coords.y + y) * output_image->width + (output_coords.x + x);

            overlap_error[error_idx] = rgb_sq_error(
                    output_image->data + output_idx, source_image->data + source_idx
            );
        }
    }

    Matrix *errors = (Matrix *) malloc(sizeof(Matrix));
    errors->data = overlap_error;
    errors->width = overlap_width;
    errors->height = overlap_height;

    return errors;
}


Matrix *transpose(Matrix *a) {
    Matrix *a_transposed = (Matrix *) malloc(sizeof(Matrix));
    a_transposed->width = a->height;
    a_transposed->height = a->width;
    a_transposed->data = (int *) malloc(a->height * a->width * sizeof(int));

    int transposed_idx = 0;
    for (int j = 0; j < a->width; ++j) {
        for (int i = 0; i < a->height; ++i) {
            a_transposed->data[transposed_idx] = a->data[i * a->width + j];
            transposed_idx++;
        }
    }

    free_matrix(a);
    return a_transposed;
}


/*
 * cut[cut_x_idx, 0:cut_y_idx] = -1;
 * cut[cut_x_idx, cut_y_idx] = 0;
 * cut[cut_x_idx, cut_y_idx:overlap_height] = 1;
 */
void mark_cut_path(Matrix *cut, int cut_x_idx, int cut_y_idx) {
    int idx = 0;
    for (; idx < cut_x_idx; idx++) {
        cut->data[cut_y_idx * cut->width + idx] = -1;
    }
    cut->data[cut_y_idx * cut->width + idx] = 0;
    idx++;
    for (; idx < cut->width; idx++) {
        cut->data[cut_y_idx * cut->width + idx] = 1;
    }
}


Matrix *min_cut(
        Image *source_image, Image *output_image, ImageCoordinates block_coords, ImageCoordinates output_coords,
        int block_size, int overlap, Direction direction
) {

    if (direction == FIRST) {
        // first block top left -> just copy
        copy_section(source_image, output_image, block_coords, output_coords, block_size, block_size);
        return 0;
    }

    int overlap_width;
    int overlap_height;
    Matrix *error_matrix;
    if (direction == ABOVE) {
        overlap_width = block_size;
        overlap_height = overlap;
        error_matrix = calc_overlap_error(source_image, output_image, block_coords, output_coords, overlap_width,
                                          overlap_height);

        // transpose so that we can compute the ABOVE and the LEFT case the same way
        error_matrix = transpose(error_matrix);
        overlap_width = error_matrix->width;
        overlap_height = error_matrix->height;

    } else if (direction == LEFT) {
        overlap_width = overlap;
        overlap_height = block_size;
        error_matrix = calc_overlap_error(source_image, output_image, block_coords, output_coords, overlap_width,
                                          overlap_height);

    } else {
        // todo: implement corner overlap case
        printf(stderr, "todo: implement corner overlap case");
        exit(1);
    }


    int *overlap_errors = error_matrix->data;


    Matrix *dp = malloc(sizeof(Matrix));
    dp->width = overlap_width;
    dp->height = overlap_height;
    dp->data = (int *) calloc(overlap_height * overlap_width, sizeof(int));

    // fill in the first row of dp with the error at that starting point
    for (int i = 0; i < overlap_width; ++i) {
        dp->data[i] = overlap_errors[i];
    }


    // compute DP table entries
    for (int i = 1; i < overlap_height; i++) {
        for (int j = 0; j < overlap_width; j++) {


            int min_path = dp->data[(i - 1) * overlap_width + j];

            if (j > 0) {
                int top_left_path = dp->data[(i - 1) * overlap_width + (j - 1)];
                min_path = top_left_path < min_path ? top_left_path : min_path;
            }

            if (j < overlap_width - 1) {
                int top_right_path = dp->data[(i - 1) * overlap_width + (j + 1)];
                min_path = top_right_path < min_path ? top_right_path : min_path;
            }

            dp->data[i * overlap_width + j] = overlap_errors[i * overlap_width + j] + min_path;
        }
    }

    free_matrix(error_matrix);

    // find min cut with backtracking
    Matrix *cut = (Matrix *) malloc(sizeof(Matrix));
    cut->width = overlap_width;
    cut->height = overlap_height;
    cut->data = (int *) calloc(overlap_height * overlap_width, sizeof(int));;

    for (int i = 0; i < overlap_width * overlap_width; ++i) {
        cut->data[i] = 1;
    }

    // find start point of backtracking
    int cut_x_idx = 0;
    int min_start_error = dp->data[(overlap_height - 1) * overlap_width];
    for (int j = 1; j < overlap_width; j++) {
        int err = dp->data[(overlap_height - 1) * overlap_width + j];

        if (err < min_start_error) {
            cut_x_idx = j;
            min_start_error = err;
        }
    }

    // initialize backtracking
    mark_cut_path(cut, cut_x_idx, overlap_height - 1);

    // backtracking and mark cut path
    for (int i = overlap_height - 1; i >= 0; i--) {
        int new_cut_x_idx = cut_x_idx;
        int min_error = dp->data[i * overlap_width + cut_x_idx];
        if (cut_x_idx < overlap_width - 1) {
            int err_top_right = dp->data[i * overlap_width + cut_x_idx + 1];
            if (err_top_right < min_error) {
                min_error = err_top_right;
                new_cut_x_idx = cut_x_idx + 1;
            }
        }

        if (cut_x_idx > 0) {
            int err_top_left = dp->data[i * overlap_width + cut_x_idx - 1];
            if (err_top_left < min_error) {
                new_cut_x_idx = cut_x_idx - 1;
            }
        }

        cut_x_idx = new_cut_x_idx;
        mark_cut_path(cut, cut_x_idx, i);
    }

    free_matrix(dp);

    if (direction == ABOVE) {
        // transpose the matrix back
        cut = transpose(cut);
    }
    return cut;
}


Image *quilting(Image *image, int block_size, int out_num_blocks, int overlap_size) {

    int im_height = image->height;
    int im_width = image->width;

    int num_blocks_h = im_height - block_size;
    int num_blocks_w = im_width - block_size;

    if (num_blocks_h < 1 || num_blocks_w < 1) {
        fprintf(stderr, "error: quilting block size (%dx%d) exceeds one of the image dimensions (%dx%d)\n",
                block_size, block_size, im_height, im_width);
        exit(1);
    }

    double tolerance = 0.3;
    int out_size = out_num_blocks * block_size - (out_num_blocks - 1) * overlap_size;

    RGB *out_data = (RGB *) malloc(out_size * out_size * sizeof(RGB));
    RGB *slice_data = (RGB *) malloc(block_size * block_size * sizeof(RGB));
    Image *out_slice = malloc(sizeof(Image));

    Image *output_image = malloc(sizeof(Image));
    output_image->data = out_data;
    output_image->width = out_size;
    output_image->height = out_size;

    int seed = RSEED;
    //srand(seed);
    srand(time(NULL));

    int *inds = (int *) malloc(2 * sizeof(int));

    for (int i = 0; i < out_num_blocks; i++) {
        for (int j = 0; j < out_num_blocks; j++) {

            int start_i = i * block_size - i * overlap_size;
            int start_j = j * block_size - j * overlap_size;

            double *errors;

            if (i == 0 && j == 0) {
                int rand_block_row = rand() % num_blocks_h;
                int rand_block_col = rand() % num_blocks_w;
                //  printf("init index - y:%d x:%d\n", rand_block_row, rand_block_col);
                copy_blocks(image->data, out_data, im_width, out_size,
                            0, 0, block_size, block_size,
                            0, 0, rand_block_row, rand_block_col);
                continue;
            } else if (j == 0) {
                copy_blocks(out_data, slice_data, out_size, block_size,
                            0, 0, block_size, overlap_size,
                            0, 0, start_i, 0);
                out_slice->data = slice_data;
                out_slice->width = block_size;
                out_slice->height = block_size;
                double *errors = calc_errors(image, out_slice, block_size, overlap_size, "above");
                find_indexes(errors, inds, num_blocks_h, num_blocks_w);
                copy_blocks(image->data, out_data, im_width, out_size,
                            0, overlap_size, block_size, block_size,
                            start_i, 0, inds[0], inds[1]);
                free(errors);
                continue;
            } else if (i == 0) {
                copy_blocks(out_data, slice_data, out_size, block_size,
                            0, 0, overlap_size, block_size,
                            0, 0, 0, start_j);
                out_slice->data = slice_data;
                out_slice->width = block_size;
                out_slice->height = block_size;
                double *errors = calc_errors(image, out_slice, block_size, overlap_size, "left");
                find_indexes(errors, inds, num_blocks_h, num_blocks_w);
                copy_blocks(image->data, out_data, im_width, out_size,
                            overlap_size, 0, block_size, block_size,
                            0, start_j, inds[0], inds[1]);
                free(errors);
                continue;
            } else {
                copy_blocks(out_data, slice_data, out_size, block_size,
                            0, 0, block_size, overlap_size,
                            0, 0, start_i, 0);
                out_slice->data = slice_data;
                out_slice->width = block_size;
                out_slice->height = block_size;
                double *errors = calc_errors(image, out_slice, block_size, overlap_size, "above");
                copy_blocks(out_data, slice_data, out_size, block_size,
                            0, 0, overlap_size, block_size,
                            0, 0, 0, start_j);
                out_slice->data = slice_data;
                out_slice->width = block_size;
                out_slice->height = block_size;
                double *errors_left = calc_errors(image, out_slice, block_size, overlap_size, "left");
                double *errors_corner = calc_errors(image, out_slice, block_size, overlap_size, "corner");
                for (int y = 0; y < num_blocks_h; y++) {
                    for (int x = 0; x < num_blocks_w; x++) {
                        int idx = y * num_blocks_w + x;
                        errors[idx] += (errors_left[idx] - errors_corner[idx]);
                    }
                }
                find_indexes(errors, inds, num_blocks_h, num_blocks_w);


//                ImageCoordinates block_coords = {inds[0], inds[1]};
//                ImageCoordinates out_coords = {start_i, start_j};
//                min_cut(image, output_image, block_coords, out_coords, block_size, overlap_size, CORNER);


                copy_blocks(image->data, out_data, im_width, out_size,
                            overlap_size, overlap_size, block_size, block_size,
                            start_i, start_j, inds[0], inds[1]);


                free(errors_left);
                free(errors_corner);
                free(errors);
            }
        }
    }

    free(slice_data);
    free(inds);


    return output_image;

}


#include "src/matrix.h"
#include "src/image.h"
#include <stdlib.h>
#include <stdio.h>
#include <immintrin.h>

/*
 * calculates the error in the overlapping section
 */
Matrix *calc_overlap_error_opt_5(
        ImageRGB *source_image, ImageRGB *output_image, ImageCoordinates block_coords,
        ImageCoordinates output_coords,
        int overlap_width, int overlap_height
) {

    int *overlap_error = (int *) malloc(overlap_width * overlap_height * sizeof(int));

    int source_idx;
    int output_idx;
    int error_idx;

    int r_error, g_error, b_error;

    for (int y = 0; y < overlap_height; y++) {
        for (int x = 0; x < overlap_width; x++) {
            error_idx = y * overlap_width + x;
            source_idx = (block_coords.y + y) * source_image->width + (block_coords.x + x);
            output_idx = (output_coords.y + y) * output_image->width + (output_coords.x + x);

            r_error = source_image->r_data[source_idx] - output_image->r_data[output_idx];
            g_error = source_image->g_data[source_idx] - output_image->g_data[output_idx];
            b_error = source_image->b_data[source_idx] - output_image->b_data[output_idx];

            r_error = r_error * r_error;
            g_error = g_error * g_error;
            b_error = b_error * b_error;


            overlap_error[error_idx] += r_error + g_error + b_error;
        }
    }

    Matrix *errors = (Matrix *) malloc(sizeof(Matrix));
    errors->data = overlap_error;
    errors->width = overlap_width;
    errors->height = overlap_height;

    return errors;
}


/*
 * cut[0:cut_x_idx, cut_y_idx] = -1;
 * cut[cut_x_idx, cut_y_idx] = 0;
 * cut[cut_x_idx+1:overlap_width, cut_y_idx] = 1;
 */
void mark_cut_path_hori_opt_5(Matrix *cut, int cut_x_idx, int cut_y_idx) {
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

/*
 * cut[cut_x_idx, 0:cut_y_idx] = -1;
 * cut[cut_x_idx, cut_y_idx] = 0;
 * cut[cut_x_idx, cut_y_idx:overlap_height] = 1;
 */
void mark_cut_path_vert_opt_5(Matrix *cut, int cut_x_idx, int cut_y_idx) {
    int idx = 0;
    for (; idx < cut_y_idx; idx++) {
        cut->data[idx * cut->width + cut_x_idx] = -1;
    }
    cut->data[idx * cut->width + cut_x_idx] = 0;
    idx++;
    for (; idx < cut->height; idx++) {
        cut->data[idx * cut->width + cut_x_idx] = 1;
    }
}


/*
 * merges cut matrices and stores result in pointer cut_0
 *
 * merge mappings:
 * 1 and  1 ->  1
 * 1 and  0 ->  0
 * 1 and -1 -> -1
 * 0 and  0 ->  0
 * 0 and -1 -> -1
 */
Matrix *merge_cut_matrix_opt_5(Matrix *cut_0, Matrix *cut_1) {
//    print_matrix(cut_0);
//    printf("\n");
//    print_matrix(cut_1);
//    printf("\n");

    if (!matrix_equal_size(cut_0, cut_1)) {
        fprintf(stderr, "matrices are not equal size, can no merge cut matrices");
    }

    int height = cut_0->height;
    int width = cut_0->width;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int v0 = cut_0->data[y * width + x];
            int v1 = cut_1->data[y * width + x];

            cut_0->data[y * width + x] = v0 < v1 ? v0 : v1;
        }
    }

    return cut_0;
}


Matrix *calc_cut_mask_left_opt_5(Matrix *error_matrix, int block_size) {
    int *overlap_errors = error_matrix->data;
    int overlap_width = error_matrix->width;
    int overlap_height = error_matrix->height;


    Matrix *dp = error_matrix;




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

//    free_matrix(error_matrix);

    // find min cut with backtracking
    Matrix *cut = (Matrix *) malloc(sizeof(Matrix));
    cut->width = block_size;
    cut->height = block_size;
    cut->data = (int *) calloc(cut->width * cut->height, sizeof(int));;


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
    mark_cut_path_hori_opt_5(cut, cut_x_idx, overlap_height - 1);

    // backtracking and mark cut path
    for (int i = overlap_height - 2; i >= 0; i--) {
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
        mark_cut_path_hori_opt_5(cut, cut_x_idx, i);
    }

    free_matrix(dp);
    return cut;
}


Matrix *calc_cut_mask_above_opt_5(Matrix *error_matrix, int block_size) {
    int *overlap_errors = error_matrix->data;
    int overlap_width = error_matrix->width;
    int overlap_height = error_matrix->height;


    Matrix *dp = error_matrix;
    int *dp_data = dp->data;

    // compute DP table entries
    for (int x = 1; x < overlap_width; x++) {
        for (int y = 0; y < overlap_height; y++) {


            int min_path = dp->data[y * overlap_width + (x - 1)];

            if (y > 0) {
                int top_left_path = dp->data[(y - 1) * overlap_width + (x - 1)];
                min_path = top_left_path < min_path ? top_left_path : min_path;
            }

            if (y < overlap_height - 1) {
                int top_right_path = dp->data[(y + 1) * overlap_width + (x - 1)];
                min_path = top_right_path < min_path ? top_right_path : min_path;
            }

            dp->data[y * overlap_width + x] = overlap_errors[y * overlap_width + x] + min_path;
        }
    }

//    free_matrix(error_matrix);

    // find min cut with backtracking
    Matrix *cut = (Matrix *) malloc(sizeof(Matrix));
    cut->width = block_size;
    cut->height = block_size;
    cut->data = (int *) calloc(cut->width * cut->height, sizeof(int));;


    // find start point of backtracking
    int cut_y_idx = 0;
    int min_start_error = dp->data[(overlap_height - 1) * overlap_width];
    for (int y = 1; y < overlap_height; y++) {
        int err = dp->data[y * overlap_width + (overlap_width - 1)];

        if (err < min_start_error) {
            cut_y_idx = 0;
            min_start_error = err;
        }
    }

    // initialize backtracking
    mark_cut_path_vert_opt_5(cut, overlap_width - 2, cut_y_idx);

    // backtracking and mark cut path
    for (int x = overlap_width - 2; x > -1; x--) {
        int new_cut_y_idx = cut_y_idx;
        int min_error = dp_data[cut_y_idx * overlap_width + x];

        if (cut_y_idx < overlap_height - 1) {
            int err_top_right = dp_data[(cut_y_idx + 1) * overlap_width + x];
            if (err_top_right < min_error) {
                min_error = err_top_right;
                new_cut_y_idx = cut_y_idx + 1;
            }
        }

        if (cut_y_idx > 0) {
            int err_top_left = dp_data[(cut_y_idx - 1) * overlap_width + x];
            if (err_top_left < min_error) {
                new_cut_y_idx = cut_y_idx;
            }
        }

        cut_y_idx = new_cut_y_idx;
        mark_cut_path_vert_opt_5(cut, x, cut_y_idx);
    }

    free_matrix(dp);
    return cut;
}


Matrix *min_cut_opt_5(
        ImageRGB *source_image, ImageRGB *output_image, ImageCoordinates block_coords,
        ImageCoordinates output_coords,
        int block_size, int overlap, Direction direction
) {

    if (direction == ABOVE) {
        Matrix *error_matrix = calc_overlap_error_opt_5(source_image, output_image, block_coords,
                                                        output_coords,
                                                        block_size,
                                                        overlap);

        Matrix *cut = calc_cut_mask_above_opt_5(error_matrix, block_size);
        return cut;

    } else if (direction == LEFT) {
        Matrix *error_matrix = calc_overlap_error_opt_5(source_image, output_image, block_coords,
                                                        output_coords,
                                                        overlap,
                                                        block_size);
        Matrix *cut = calc_cut_mask_left_opt_5(error_matrix, block_size);
        return cut;

    } else if (direction == CORNER) {
        Matrix *error_matrix_left = calc_overlap_error_opt_5(source_image, output_image,
                                                             block_coords,
                                                             output_coords,
                                                             overlap,
                                                             block_size);
        Matrix *cut_left = calc_cut_mask_left_opt_5(error_matrix_left, block_size);

        Matrix *error_matrix_above = calc_overlap_error_opt_5(source_image, output_image,
                                                              block_coords,
                                                              output_coords,
                                                              block_size,
                                                              overlap);
        Matrix *cut_above = calc_cut_mask_above_opt_5(error_matrix_above, block_size);

        Matrix *cut = merge_cut_matrix_opt_5(cut_left, cut_above);
        free_matrix(cut_above);
        return cut;
    } else {
        fprintf(stderr, "invalid cut direction");
        exit(1);
    }
}

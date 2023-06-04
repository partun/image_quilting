#include "src/matrix.h"
#include "src/image.h"
#include <stdlib.h>
#include <stdio.h>
#include <immintrin.h>

#define UNROLL 16


/*
 * calculates the error in the overlapping section
 */
Matrix *calc_overlap_error_opt_11(
        ImageRGB *source_image, ImageRGB *output_image, ImageCoordinates block_coords,
        ImageCoordinates output_coords,
        int overlap_width, int overlap_height
) {

    ColorV *src_r_data = source_image->r_data;
    ColorV *src_g_data = source_image->g_data;
    ColorV *src_b_data = source_image->b_data;

    ColorV *out_r_data = output_image->r_data;
    ColorV *out_g_data = output_image->g_data;
    ColorV *out_b_data = output_image->b_data;


    Matrix *errors = (Matrix *) malloc(sizeof(Matrix));
    errors->data = (int *) malloc(overlap_width * overlap_height * sizeof(int));;
    errors->width = overlap_width;
    errors->height = overlap_height;


    int *overlap_error = errors->data;

    int source_idx;
    int output_idx;
    int error_idx;

    int r_error_0, g_error_0, b_error_0;

    for (int y = 0; y < overlap_height; y++) {
        error_idx = y * overlap_width;
        source_idx = (block_coords.y + y) * source_image->width + block_coords.x;
        output_idx = (output_coords.y + y) * output_image->width + output_coords.x;

        int x = 0;

        for (; x < overlap_width - UNROLL + 1; x += UNROLL) {
            __m128i r_src = _mm_loadu_si128((const __m128i *) (src_r_data + source_idx));
            __m128i g_src = _mm_loadu_si128((const __m128i *) (src_g_data + source_idx));
            __m128i b_src = _mm_loadu_si128((const __m128i *) (src_b_data + source_idx));

            __m128i r_out = _mm_loadu_si128((const __m128i *) (out_r_data + output_idx));
            __m128i g_out = _mm_loadu_si128((const __m128i *) (out_g_data + output_idx));
            __m128i b_out = _mm_loadu_si128((const __m128i *) (out_b_data + output_idx));

            __m256i r_src_int_0 = _mm256_cvtepu8_epi32(r_src);
            __m256i g_src_int_0 = _mm256_cvtepu8_epi32(g_src);
            __m256i b_src_int_0 = _mm256_cvtepu8_epi32(b_src);

            __m256i r_out_int_0 = _mm256_cvtepu8_epi32(r_out);
            __m256i g_out_int_0 = _mm256_cvtepu8_epi32(g_out);
            __m256i b_out_int_0 = _mm256_cvtepu8_epi32(b_out);


            r_src = (__m128i) _mm_permute_pd((__m128d) r_src, 0b01);
            g_src = (__m128i) _mm_permute_pd((__m128d) g_src, 0b01);
            b_src = (__m128i) _mm_permute_pd((__m128d) b_src, 0b01);

            r_out = (__m128i) _mm_permute_pd((__m128d) r_out, 0b01);
            g_out = (__m128i) _mm_permute_pd((__m128d) g_out, 0b01);
            b_out = (__m128i) _mm_permute_pd((__m128d) b_out, 0b01);

            __m256i r_src_int_1 = _mm256_cvtepu8_epi32(r_src);
            __m256i g_src_int_1 = _mm256_cvtepu8_epi32(g_src);
            __m256i b_src_int_1 = _mm256_cvtepu8_epi32(b_src);

            __m256i r_out_int_1 = _mm256_cvtepu8_epi32(r_out);
            __m256i g_out_int_1 = _mm256_cvtepu8_epi32(g_out);
            __m256i b_out_int_1 = _mm256_cvtepu8_epi32(b_out);


            r_src_int_0 = _mm256_sub_epi32(r_src_int_0, r_out_int_0);
            g_src_int_0 = _mm256_sub_epi32(g_src_int_0, g_out_int_0);
            b_src_int_0 = _mm256_sub_epi32(b_src_int_0, b_out_int_0);

            r_src_int_0 = _mm256_mullo_epi32(r_src_int_0, r_src_int_0);
            g_src_int_0 = _mm256_mullo_epi32(g_src_int_0, g_src_int_0);
            b_src_int_0 = _mm256_mullo_epi32(b_src_int_0, b_src_int_0);

            r_src_int_1 = _mm256_sub_epi32(r_src_int_1, r_out_int_1);
            g_src_int_1 = _mm256_sub_epi32(g_src_int_1, g_out_int_1);
            b_src_int_1 = _mm256_sub_epi32(b_src_int_1, b_out_int_1);

            r_src_int_1 = _mm256_mullo_epi32(r_src_int_1, r_src_int_1);
            g_src_int_1 = _mm256_mullo_epi32(g_src_int_1, g_src_int_1);
            b_src_int_1 = _mm256_mullo_epi32(b_src_int_1, b_src_int_1);


            __m256i err_0 = _mm256_add_epi32(r_src_int_0, g_src_int_0);
            err_0 = _mm256_add_epi32(err_0, b_src_int_0);

            __m256i err_1 = _mm256_add_epi32(r_src_int_1, g_src_int_1);
            err_1 = _mm256_add_epi32(err_1, b_src_int_1);

            _mm256_storeu_si256((__m256i *) (overlap_error + error_idx), err_0);
            _mm256_storeu_si256((__m256i *) (overlap_error + error_idx + 8), err_1);

            error_idx += UNROLL;
            source_idx += UNROLL;
            output_idx += UNROLL;
        }

        for (; x < overlap_width; x++) {
            r_error_0 = src_r_data[source_idx] - out_r_data[output_idx];
            g_error_0 = src_g_data[source_idx] - out_g_data[output_idx];
            b_error_0 = src_b_data[source_idx] - out_b_data[output_idx];

            r_error_0 = r_error_0 * r_error_0;
            g_error_0 = g_error_0 * g_error_0;
            b_error_0 = b_error_0 * b_error_0;

            overlap_error[error_idx] += r_error_0 + g_error_0 + b_error_0;

            error_idx++;
            source_idx++;
            output_idx++;
        }
    }


    return errors;
}


/*
 * cut[0:cut_x_idx, cut_y_idx] = 2;
 * cut[cut_x_idx, cut_y_idx] = 1;
 * cut[cut_x_idx+1:overlap_width, cut_y_idx] = 0;
 */
void mark_cut_path_hori_opt_11(CutMatrix *cut, int cut_x_idx, int cut_y_idx) {
    char *data = cut->data;
    int width = cut->width;
    int base_idx = cut_y_idx * width;

    int idx = 0;
    for (; idx < cut_x_idx; idx++) {
        data[base_idx + idx] = 2;
    }
    data[base_idx + idx] = 1;
}

/*
 * cut[cut_x_idx, 0:cut_y_idx] = 2;
 * cut[cut_x_idx, cut_y_idx] = 1;
 * cut[cut_x_idx, cut_y_idx:overlap_height] = 0;
 */
void mark_cut_path_vert_opt_11(CutMatrix *cut, int cut_x_idx, int cut_y_idx) {
    int idx = 0;
    for (; idx < cut_y_idx; idx++) {
        cut->data[idx * cut->width + cut_x_idx] = 2;
    }
    cut->data[idx * cut->width + cut_x_idx] = 1;
}


/*
 * merges cut matrices and stores result in pointer cut_0
 *
 * merge mappings:
 * 1 and  2 ->  2
 * 1 and  1 ->  1
 * 1 and  0 ->  1
 * 2 and  0 ->  2
 * 0 and  0 ->  0
 */
CutMatrix *merge_cut_matrix_opt_11(CutMatrix *cut_0, CutMatrix *cut_1) {

    if (cut_0->width != cut_1->width || cut_0->height != cut_1->height) {
        fprintf(stderr, "matrices are not equal size, can no merge cut matrices");
    }

    int height = cut_0->height;
    int width = cut_0->width;

    char *cut_0_data = cut_0->data;
    char *cut_1_data = cut_1->data;

    int i = 0;
    for (; i < width * height - 32 + 1; i += 32) {
        __m256i m0 = _mm256_loadu_si256((const __m256i *) (cut_0_data + i));
        __m256i m1 = _mm256_loadu_si256((const __m256i *) (cut_1_data + i));

        __m256i mask = _mm256_cmpgt_epi8(m0, m1);

        m0 = _mm256_blendv_epi8(m1, m0, mask);
        _mm256_storeu_si256((__m256i *) (cut_0_data + i), m0);
    }

    for (; i < width * height; ++i) {
        char v0 = cut_0_data[i];
        char v1 = cut_1_data[i];

        cut_0_data[i] = v0 > v1 ? v0 : v1;
    }
    return cut_0;
}


CutMatrix *calc_cut_mask_left_opt_11(Matrix *error_matrix, int block_size) {
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
    CutMatrix *cut = (CutMatrix *) malloc(sizeof(CutMatrix));
    cut->width = block_size;
    cut->height = block_size;
    cut->data = (char *) calloc(cut->width * cut->height, sizeof(char));


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
    mark_cut_path_hori_opt_11(cut, cut_x_idx, overlap_height - 1);

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
        mark_cut_path_hori_opt_11(cut, cut_x_idx, i);
    }

    free_matrix(dp);
    return cut;
}


CutMatrix *calc_cut_mask_above_opt_11(Matrix *error_matrix, int block_size) {
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


    // find min cut with backtracking
    CutMatrix *cut = (CutMatrix *) malloc(sizeof(CutMatrix));
    cut->width = block_size;
    cut->height = block_size;
    cut->data = (char *) calloc(cut->width * cut->height, sizeof(char));


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
    mark_cut_path_vert_opt_11(cut, overlap_width - 2, cut_y_idx);

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
        mark_cut_path_vert_opt_11(cut, x, cut_y_idx);
    }

    free_matrix(dp);
    return cut;
}


CutMatrix *min_cut_opt_11(
        ImageRGB *source_image, ImageRGB *output_image, ImageCoordinates block_coords,
        ImageCoordinates output_coords,
        int block_size, int overlap, Direction direction
) {

    if (direction == ABOVE) {
        Matrix *error_matrix = calc_overlap_error_opt_11(source_image, output_image, block_coords,
                                                         output_coords,
                                                         block_size,
                                                         overlap);

        CutMatrix *cut = calc_cut_mask_above_opt_11(error_matrix, block_size);
        return cut;

    } else if (direction == LEFT) {
        Matrix *error_matrix = calc_overlap_error_opt_11(source_image, output_image, block_coords,
                                                         output_coords,
                                                         overlap,
                                                         block_size);
        CutMatrix *cut = calc_cut_mask_left_opt_11(error_matrix, block_size);
        return cut;

    } else if (direction == CORNER) {
        Matrix *error_matrix_left = calc_overlap_error_opt_11(source_image, output_image,
                                                              block_coords,
                                                              output_coords,
                                                              overlap,
                                                              block_size);
        CutMatrix *cut_left = calc_cut_mask_left_opt_11(error_matrix_left, block_size);

        Matrix *error_matrix_above = calc_overlap_error_opt_11(source_image, output_image,
                                                               block_coords,
                                                               output_coords,
                                                               block_size,
                                                               overlap);
        CutMatrix *cut_above = calc_cut_mask_above_opt_11(error_matrix_above, block_size);

        CutMatrix *cut = merge_cut_matrix_opt_11(cut_left, cut_above);
        free_cut_matrix(cut_above);
        return cut;
    } else {
        fprintf(stderr, "invalid cut direction");
        exit(1);
    }
}

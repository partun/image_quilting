#include "src/opt_11/calc_errors_opt_11.h"
#include "src/opt_11/min_cut_opt_11.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define RSEED 8


/*
 * copies block from source image to out image
 */
void copy_block_opt_11(
        ImageRGB *src, ImageCoordinates src_coord, ImageRGB *out,
        ImageCoordinates out_coord, int block_size
) {
    unsigned int src_idx;
    unsigned int out_idx;

    for (int y = 0; y < block_size; ++y) {
        for (int x = 0; x < block_size; ++x) {
            src_idx = (y + src_coord.y) * src->width + x + src_coord.x;
            out_idx = (y + out_coord.y) * out->width + x + out_coord.x;

            out->r_data[out_idx] = src->r_data[src_idx];
            out->g_data[out_idx] = src->g_data[src_idx];
            out->b_data[out_idx] = src->b_data[src_idx];
        }
    }
}

void mask_copy_block_opt_11(
        ImageRGB *source_image, ImageRGB *output_image, CutMatrix *mask,
        ImageCoordinates block_coords,
        ImageCoordinates output_coords, int block_size
) {

    int source_idx;
    int output_idx;
    int mask_idx;

    for (int y = 0; y < block_size; y++) {
        for (int x = 0; x < block_size; x++) {
            mask_idx = y * mask->width + x;
            source_idx =
                    (block_coords.y + y) * source_image->width + (block_coords.x + x);
            output_idx =
                    (output_coords.y + y) * output_image->width + (output_coords.x + x);

            if (mask->data[mask_idx] == 0) {
                output_image->r_data[output_idx] = source_image->r_data[source_idx];
                output_image->g_data[output_idx] = source_image->g_data[source_idx];
                output_image->b_data[output_idx] = source_image->b_data[source_idx];
            } else if (mask->data[mask_idx] == 1) {
                output_image->b_data[output_idx] =
                        output_image->b_data[output_idx] / 2 +
                        source_image->b_data[source_idx] / 2;
                output_image->g_data[output_idx] =
                        output_image->g_data[output_idx] / 2 +
                        source_image->g_data[source_idx] / 2;
                output_image->r_data[output_idx] =
                        output_image->r_data[output_idx] / 2 +
                        source_image->r_data[source_idx] / 2;
            }
        }
    }
}

ImageRGB *quilting_opt_11(
        ImageRGB *image, int block_size, int out_blocks,
        int overlap_size
) {
    ColorOrder color_order = get_image_color_order_opt_11(image);
    permute_image_colors(image, color_order);
//    print_image_order(color_order);
//    print_image_order(get_image_color_order_opt_11(image));
    srand(time(NULL));
    if (block_size < overlap_size) {
        fprintf(stderr,
                "error: quilting block size (%d) is smaller than the overlap size "
                "(%d)\n",
                block_size, overlap_size);
        exit(1);
    }

    if (image->height < block_size || image->width < block_size) {
        fprintf(stderr,
                "error: quilting block size (%dx%d) exceeds one of the image "
                "dimensions (%dx%d)\n",
                block_size, block_size, image->height, image->width);
        exit(1);
    }

    // allocations for calc error function
    int *errors = (int *) malloc(
            (image->width - block_size) * (image->height - block_size) * sizeof(int));

    int out_size = out_blocks * (block_size - overlap_size) + overlap_size;
    ImageRGB *out_im = (ImageRGB *) malloc(sizeof(ImageRGB));
    out_im->width = out_size;
    out_im->height = out_size;
    out_im->r_data = (ColorV *) malloc(out_size * out_size * sizeof(ColorV));
    out_im->g_data = (ColorV *) malloc(out_size * out_size * sizeof(ColorV));
    out_im->b_data = (ColorV *) malloc(out_size * out_size * sizeof(ColorV));
    
    ImageCoordinates *im_coords_matrix = (ImageCoordinates *) malloc(
            (out_blocks) * (out_blocks) * sizeof(ImageCoordinates));

    for (int y = 0; y < out_blocks; y++) {
        for (int x = 0; x < out_blocks; x++) {

            // coordinates of the next block in the output image
            ImageCoordinates out_coord = {x * (block_size - overlap_size),
                                          y * (block_size - overlap_size)};

            if (x == 0 && y == 0) {
                // case: corner
                // start out image with a random block of the src image

                ImageCoordinates src_coord = {rand() % (image->width - block_size),
                                              rand() % (image->height - block_size)};
                                              
                copy_block_opt_11(image, src_coord, out_im, out_coord, block_size);
                im_coords_matrix[0]=src_coord;
                
            } else if (y == 0) {
                // case left overlap
                calc_errors_opt_11(errors, image, out_im, out_coord, block_size,
                                   overlap_size, LEFT,im_coords_matrix[0], im_coords_matrix[x-1]);
                ImageCoordinates src_coord = find_best_block_opt_11(errors, image,
                                                                    block_size);
                CutMatrix *cut = min_cut_opt_11(
                        image,
                        out_im,
                        src_coord,
                        out_coord,
                        block_size,
                        overlap_size,
                        LEFT
                );
                mask_copy_block_opt_11(
                        image,
                        out_im,
                        cut,
                        src_coord,
                        out_coord,
                        block_size
                );
                free_cut_matrix(cut);
                im_coords_matrix[x]=src_coord;
            } else if (x == 0) {
                // case above overlap
                calc_errors_opt_11(errors, image, out_im, out_coord, block_size,
                                   overlap_size, ABOVE, im_coords_matrix[x+(y-1)*out_blocks],
                                   im_coords_matrix[x-1+(y)*out_blocks]);
                ImageCoordinates src_coord = find_best_block_opt_11(errors, image,
                                                                    block_size);
                CutMatrix *cut = min_cut_opt_11(
                        image,
                        out_im,
                        src_coord,
                        out_coord,
                        block_size,
                        overlap_size,
                        ABOVE
                );
                mask_copy_block_opt_11(
                        image,
                        out_im,
                        cut,
                        src_coord,
                        out_coord,
                        block_size
                );
                free_cut_matrix(cut);
                im_coords_matrix[x+y*out_blocks]=src_coord;
            } else {
                // case corner overlap
                calc_errors_opt_11(errors, image, out_im, out_coord, block_size,
                                   overlap_size, CORNER, im_coords_matrix[x+(y-1)*out_blocks],
                                   im_coords_matrix[x-1+(y)*out_blocks]);
                ImageCoordinates src_coord = find_best_block_opt_11(errors, image,
                                                                    block_size);
                CutMatrix *cut = min_cut_opt_11(
                        image,
                        out_im,
                        src_coord,
                        out_coord,
                        block_size,
                        overlap_size,
                        CORNER
                );
                mask_copy_block_opt_11(
                        image,
                        out_im,
                        cut,
                        src_coord,
                        out_coord,
                        block_size
                );
                free_cut_matrix(cut);
                im_coords_matrix[x+y*out_blocks]=src_coord;
            }
        }
    }

    free(errors);
    free(im_coords_matrix);
    permute_image_colors(image, color_order);
    permute_image_colors(out_im, color_order);
    return out_im;
}

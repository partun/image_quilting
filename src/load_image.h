#ifndef TEAM08_LOAD_IMAGE_H
#define TEAM08_LOAD_IMAGE_H

#include "image.h"


Image *read_image(char *image_path);

ImageRGB *read_image_rgb(char *image_path);

void store_image(Image *img, char *image_path);

void store_image_rgb(ImageRGB *img, char *image_path);

#endif //TEAM08_LOAD_IMAGE_H

/* image.c */

#include <stdlib.h>

#include "image.hpp"


void
Image_Manager::image_destroy (Image *image)
{
  free (image->data);
  free (image);
}

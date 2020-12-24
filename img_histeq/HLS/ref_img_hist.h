#include "my_defines.h"

void ref_histogram_equaliz3( uint11 width, uint11 height, uint25 out_cdf[GRAY_LEVELS], uint19 hist[GRAY_LEVELS],
  					RGB_t inp_img[MAX_WIDTH*MAX_HEIGHT], RGB_t out_img[MAX_WIDTH*MAX_HEIGHT]);
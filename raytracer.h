#include <stdio.h>
#include <stdlib.h>
#include <wand/MagickWand.h>

#define W_RES 1008
#define H_RES 567

typedef struct {
  double r;
  double g;
  double b;
} COLOR;

void createImage();
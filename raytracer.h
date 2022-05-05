#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <wand/MagickWand.h>

#define W_RES 1008
#define H_RES 567

long double x_max = 0;
long double x_min = 0;
long double y_max = 0;
long double y_min = 0;

long double x_e = 0;
long double y_e = 0;
long double z_e = 0;

typedef struct {
  double r;
  double g;
  double b;
} COLOR;

typedef struct {
  long double x;
  long double y;
  long double z;
} VECTOR;

void createImage();
void raytracing();
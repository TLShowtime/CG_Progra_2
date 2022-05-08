#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <wand/MagickWand.h>

#define W_RES 1008
#define H_RES 567

long double x_max = 200;
long double x_min = -100;
long double y_max = 200;
long double y_min = -100;

long double x_e = 0;
long double y_e = 0;
long double z_e = 0;

long double x_p = 0;
long double y_p = 10;
long double z_p = 10;

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

typedef struct{
  COLOR color;
  VECTOR punto;
  VECTOR normal;
  bool existe;
  long double t;
} intersection;

typedef struct {
  long double x_c;
  long double y_c;
  long double z_c;
  long double r;
  COLOR color;
  /*Formula:  (x - xc)² + (y - yc)² + (z - zc)² - r² = 0
              (ojo.x + t*xd - xc)² + (ojo.y + t*yd - yc)² + (ojo.z + t*zd - zc)² - r² = 0
              t²(xd)² + t(2(xd(ojo.x - xc))) + (ojo.x - xc)² +
              t²(yd)² + t(2(yd(ojo.y - yc))) + (ojo.y - yc)² +
              t²(zd)² + t(2(zd(ojo.z - zc))) + (ojo.z - zc)² - r² = 0

              t²a + tb + c
              t = (-b +- sqrt(b² - 4ac))/2a 
  */
} sphere;

void createImage();
void raytracing();

COLOR de_que_color(long double x_d, long double y_d, long double z_d);

intersection F_inter();

intersection calcInterEsfera();
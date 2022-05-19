#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <wand/MagickWand.h>
#include "figuras.h"

#define W_RES 1008
#define H_RES 567
#define LISTA_SIZE 2

long double x_max = 400;
long double x_min = -400;
long double y_max = 400;
long double y_min = -400;

long double x_e = 0;
long double y_e = 0;
long double z_e = -100;

long double x_p = 0;
long double y_p = 0;
long double z_p = 0;

typedef struct {
  long double x;
  long double y;
  long double z;
} VECTOR;

typedef struct{
  COLOR color;
  VECTOR punto;
  VECTOR normal;
  long double t;
} intersection;



void createImage();
void raytracing();

COLOR de_que_color(long double x_d, long double y_d, long double z_d);

intersection* F_inter();

intersection* calcInterEsfera();


/*
####################################################################################################################
====================================================================================================================
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
====================================================================================================================
####################################################################################################################
*/

sphere esfera1 = {.x_c = 0, .y_c = 0, .z_c = 20, .r = 60, .color = {.r=255, .g=0, .b=0} };

sphere esfera2 = {.x_c = 70, .y_c = 10, .z_c = 10, .r = 50, .color = {.r=0, .g=255, .b=0} };

sphere* listaObjetos[LISTA_SIZE] = {
  &esfera1,&esfera2
};
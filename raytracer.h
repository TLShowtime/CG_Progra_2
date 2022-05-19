#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <wand/MagickWand.h>
#include "figuras.h"

#define W_RES 1008
#define H_RES 567
#define LISTA_SIZE 3

long double x_max = W_RES/4;
long double x_min = -(W_RES/4);
long double y_max = H_RES/4;
long double y_min = -(H_RES/4);

long double x_e = 0;
long double y_e = 0;
long double z_e = -(H_RES/2);

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

COLOR de_que_color(VECTOR ojo, VECTOR direccion);

intersection* F_inter(VECTOR a, VECTOR d);

intersection* calcInterEsfera(sphere* esfera, VECTOR eye, VECTOR d);


/*
####################################################################################################################
====================================================================================================================
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
====================================================================================================================
####################################################################################################################
*/

sphere esfera1 = {.x_c = 40, .y_c = 0, .z_c = 50, .r = 60, .color = {.r=255, .g=0, .b=0} };

sphere esfera2 = {.x_c = -40, .y_c = 0, .z_c = 10, .r = 30, .color = {.r=0, .g=255, .b=0} };
sphere esfera3 = {.x_c = 80, .y_c = -50, .z_c = 100, .r = 100, .color = {.r=0, .g=0, .b=255} };

sphere* listaObjetos[LISTA_SIZE] = {
  &esfera1, &esfera2, &esfera3
};
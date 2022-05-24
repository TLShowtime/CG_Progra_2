#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <wand/MagickWand.h>
#include "figuras.h"

#define W_RES 1008
#define H_RES 567
#define LISTA_SIZE 30
#define EPSILON 0.0005

int lista_length = LISTA_SIZE;
int luces_length = LISTA_SIZE;

long double x_max = W_RES/4;
long double x_min = -(W_RES/4);
long double y_max = H_RES/4;
long double y_min = -(H_RES/4);

long double x_e = 0;
long double y_e = 0;
long double z_e = -(H_RES/2);

long double I_A = 0.30;

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
  long double K_D;
  long double K_A;
} intersection;



void createImage();
void raytracing();

COLOR de_que_color(VECTOR ojo, VECTOR direccion);

intersection* F_inter(VECTOR a, VECTOR d);

intersection* calcInterEsfera(sphere* esfera, VECTOR eye, VECTOR d);

long double producto_punto(VECTOR a, VECTOR b);

/*
####################################################################################################################
====================================================================================================================
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
====================================================================================================================
####################################################################################################################
*/

sphere* listaObjetos[LISTA_SIZE];
light* listaLuces[LISTA_SIZE];

void loadFiguras();
void loadLuces();
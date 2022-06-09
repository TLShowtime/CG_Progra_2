#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <wand/MagickWand.h>
#include "figuras.h"

#define W_RES 1008
#define H_RES 567
#define LISTA_SIZE 10000
#define EPSILON 0.0005

int luces_length = LISTA_SIZE;
int figuras_length = 0;

long double x_max = W_RES/4;
long double x_min = -(W_RES/4);
long double y_max = H_RES/4;
long double y_min = -(H_RES/4);

long double x_e = 0;
long double y_e = 0;
long double z_e = -(H_RES/2) - 200;

long double I_A = 0.43;

typedef struct {
  long double x;
  long double y;
  long double z;
} VECTOR;

typedef struct{
  COLOR color;
  VECTOR punto;
  VECTOR normal;
  figura* figura; 
  long double t;
  long double K_D;
  long double K_A;
} intersection;



void createImage();
void raytracing();

COLOR de_que_color(VECTOR ojo, VECTOR direccion);

intersection* F_inter(VECTOR a, VECTOR d);

intersection* calcInterEsfera(sphere* esfera, VECTOR eye, VECTOR d);
intersection* calcInterPoligono(porygon* poligono, VECTOR eye, VECTOR d);
bool calcularPuntoEnPoligono(porygon* poligono, VECTOR punto);
bool contar_pared(two_point punto1, two_point punto2);

long double producto_punto(VECTOR a, VECTOR b);




/*
####################################################################################################################
====================================================================================================================
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
====================================================================================================================
####################################################################################################################
*/

sphere* listaEsferas[LISTA_SIZE];
light* listaLuces[LISTA_SIZE];
porygon* listaPoligonos[LISTA_SIZE];

figura* listaFiguras[LISTA_SIZE]; // Se usa la lista con todas las figuras, esferas y poligonos

void loadFiguras();
void loadLuces();
void loadPoligonos();

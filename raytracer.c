#include "raytracer.h"

COLOR **buffer;
COLOR background = {.r=0, .g=0, .b=0};

// Crea la imagen en base al buffer .avs
void createImage(){
    size_t x, y, 
    row_width,
    width = W_RES,
    height = H_RES;

    MagickWand *wand;
    PixelWand *bg;
    PixelIterator *iter;
    PixelWand **row;

    // Inicializa MagickWand
    MagickWandGenesis();
    wand = NewMagickWand();
    bg = NewPixelWand();

    PixelSetColor(bg, "WHITE");
    MagickNewImage(wand, width, height, bg);

    bg = DestroyPixelWand(bg);
    
    iter = NewPixelIterator(wand);
    for (y = 0; y < H_RES; ++ y) {
        row = PixelGetNextIteratorRow(iter, &row_width);
        for (x = 0; x < row_width; ++ x) {
            PixelSetRed(row[x], buffer[y][x].r);
            PixelSetGreen(row[x], buffer[y][x].g);
            PixelSetBlue(row[x], buffer[y][x].b);
        }
        PixelSyncIterator(iter);
    }
    iter = DestroyPixelIterator(iter);

    MagickWriteImage(wand, "imagen.avs");
    MagickWriteImage(wand, "imagen.jpg");

    // Termina la libreria
    wand = DestroyMagickWand(wand);
    MagickWandTerminus();
};

void raytracing(){
    int i, j;
    long double x_w, y_w,z_w,
                L,
                x_d, y_d, z_d;
    COLOR pixel;// Le quité el puntero para probar
    z_w = 0;

    for (i = 0; i < W_RES; i++) {
        for (j = 0; j < H_RES; j++) {
            x_w = (i + 1/2) * (x_max - x_min)/W_RES + x_min;
            y_w = (j + 1/2) * (y_max - y_min)/H_RES + y_min;

            L = sqrtl((x_w - x_e)*(x_w - x_e) + 
                     (y_w - y_e)*(y_w - y_e) +   
                     (z_w - z_e)*(z_w - z_e));
                
            x_d = (x_w - x_e)/L;
            y_d = (y_w - y_e)/L;
            z_d = (z_w - z_e)/L;
            pixel = de_que_color(x_d, y_d, z_d);
            buffer[j][i].r = pixel.r;
            buffer[j][i].g = pixel.g;
            buffer[j][i].b = pixel.b;
        }
    }
};

/*
#########################################################################################################################################
-----------------------------------------------------------------------------------------------------------------------------------------
#########################################################################################################################################
*/

COLOR de_que_color(long double x_d, long double y_d, long double z_d){
  COLOR color;
  VECTOR L;
  intersection* inter = F_inter(x_d, y_d, z_d);
  if (!inter){
      color = background;
  }
  else{
      color = inter->color;
      //L.x = x_p - inter->normal.x; // comentado para probar lo primero
      //L.y = y_p - inter->normal.y;
      //L.z = z_p - inter->normal.z;
      free(inter);
  }
  return color;
};

/*
#########################################################################################################################################
-----------------------------------------------------------------------------------------------------------------------------------------
#########################################################################################################################################
*/

intersection* F_inter(long double x_d, long double y_d, long double z_d){
  intersection* inter;
  intersection* near;
  //inter.existe = false;
  inter = NULL;
  near = NULL;
  long double tmin = 100000000000;// = 1/0;
  for (int i = 0; i < LISTA_SIZE; i++){ 
    inter = calcInterEsfera(listaObjetos[i], x_d, y_d, z_d); 
    if (inter && inter->t < tmin){
      tmin = inter->t;// tmin = d a inter;
      near = inter;//inter = inter con obj;
    }
  }
  return near;
};

/*
#########################################################################################################################################
-----------------------------------------------------------------------------------------------------------------------------------------
#########################################################################################################################################
*/

intersection* calcInterEsfera(sphere* esfera, long double x_d, long double y_d, long double z_d){
    long double b, c, t, t1, t2, discriminante;
    intersection* inter;
    inter = NULL;
    b = 2*( x_d*(x_e - esfera->x_c) + y_d*(y_e - esfera->y_c) + z_d*(z_e - esfera->z_c));
    c = powl((x_e - esfera->x_c), 2) + powl((y_e - esfera->y_c), 2) + powl((z_e - esfera->z_c), 2) - powl((esfera->r), 2);
    discriminante = b*b - 4*c;

    if (discriminante < 0){
        return inter;
    }
    else if (discriminante == 0.0){
        t = -b / 2;
    } else {
        t1 = (-b + sqrtl(discriminante))/2;
        t2 = (-b - sqrtl(discriminante))/2;
        if (t1 <= t2 && t1 > 0){
            t = t1;
        }
        else{
            t = t2;
        }
    }
    inter = malloc(sizeof(intersection));
    inter->punto.x = x_e + t*x_d;
    inter->punto.y = y_e + t*y_d;
    inter->punto.z = z_e + t*z_d;
    
    inter->color = esfera->color;
    inter->t = t;
    inter->normal.x = (inter->punto.x - esfera->x_c)/esfera->r;
    inter->normal.y = (inter->punto.y - esfera->y_c)/esfera->r;
    inter->normal.z = (inter->punto.z - esfera->z_c)/esfera->r;
    return inter;
};

/*
#########################################################################################################################################
-----------------------------------------------------------------------------------------------------------------------------------------
#########################################################################################################################################
*/

int main (){
    int i, j;
    
    buffer = (COLOR **)malloc(H_RES * sizeof(COLOR*));
    for (i = 0; i < W_RES; i++) 
        {
        buffer[i] = (COLOR *)malloc(W_RES * sizeof(COLOR));
        }
    
    for (i = 0; i < H_RES; i++) 
        {
        for (j = 0; j < W_RES; j++) 
            {
                buffer[i][j].r = 0;
                buffer[i][j].g = 0;
                buffer[i][j].b = 0;
            }
        }
    raytracing();
    createImage();
    printf("Todo está sirviendo :)\n");
    return 0;
}

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

    for (i = 0; i < H_RES; i++) 
        {
        for (j = 0; j < W_RES; j++) 
            {
                x_w = (i + 1/2) * (x_max - x_min)/H_RES + x_min;
                y_w = (j + 1/2) * (y_max - y_min)/W_RES + y_min;

                L = sqrt((x_w - x_e)*(x_w - x_e) + 
                     (y_w - y_e)*(y_w - y_e) +   
                     (z_w - z_e)*(z_w - z_e));
                
                x_d = (x_w - x_e)/L;
                y_d = (y_w - y_e)/L;
                z_d = (z_w - z_e)/L;
                //printf("x_d: %Lf, y_d: %Lf, z_d: %Lf\n", x_d, y_d, z_d);
                pixel = de_que_color(x_d, y_d, z_d);
                buffer[i][j].r = pixel.r;
                buffer[i][j].g = pixel.g;
                buffer[i][j].b = pixel.b;
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
  intersection inter = F_inter(x_d, y_d, z_d);
  L.x = x_p - inter.normal.x;
  L.y = y_p - inter.normal.y;
  L.z = z_p - inter.normal.z;
  if (!inter.existe)
    color = background;
  else
    color = inter.color;
  return color;
};

/*
#########################################################################################################################################
-----------------------------------------------------------------------------------------------------------------------------------------
#########################################################################################################################################
*/

intersection F_inter(long double x_d, long double y_d, long double z_d){
  intersection inter, near;
  sphere esfera;
  long double tmin = 100000000000;// = 1/0;
  for (int i = 0; i < 6; i++){ //cambiar el 6 por la cantidad de objetos en la escena
    inter = calcInterEsfera(esfera, x_d, y_d, z_d); // cambiar esfera por la esfera i en la lista de esferas
    if (inter.existe && inter.t < tmin){
      tmin = inter.t;// tmin = d a inter;
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

intersection calcInterEsfera(sphere esfera, long double x_d, long double y_d, long double z_d){
    long double b, c, t, t1, t2, discriminante;
    intersection inter;
    b = 2*(x_d*(x_e - esfera.x_c) + y_d*(y_e - esfera.y_c) + z_d*(z_e - esfera.z_c));
    c = pow((x_e - esfera.x_c), 2) + pow((y_e - esfera.y_c), 2) + pow((y_e - esfera.y_c), 2);
    discriminante = sqrt(b*b - 4*c);
    if (discriminante < 0){
        inter.existe = false;
        return inter;
    }
    t1 = (-b + discriminante)/2;
    t2 = (-b - discriminante)/2;
    if (t1 <= t2 && t1 > 0){
        t = t1;
    }
    else{
        t = t2;
    }
    inter.punto.x = x_e + t*x_d;
    inter.punto.y = y_e + t*y_d;
    inter.punto.z = z_e + t*z_d;
    inter.existe = true;
    inter.t = t;
    inter.normal.x = (inter.punto.x - esfera.x_c)/esfera.r;
    inter.normal.y = (inter.punto.y - esfera.y_c)/esfera.r;
    inter.normal.z = (inter.punto.z - esfera.z_c)/esfera.r;
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
                buffer[i][j].r = 255;
                buffer[i][j].g = 0;
                buffer[i][j].b = 0;
            }
        }
    
    raytracing();
    printf("Todo está sirviendo :)\n");
    createImage();
    return 0;
}

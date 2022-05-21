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
    VECTOR vector_ojo = {.x = x_e, .y = y_e, .z = z_e};

    for (i = 0; i < W_RES; i++) {
        for (j = 0; j < H_RES; j++) {
            x_w = (i + 1/2) * (x_max - x_min)/W_RES + x_min;
            y_w = (j + 1/2) * (y_max - y_min)/H_RES + y_min;
            z_w = 0;
            L = sqrtl((x_w - x_e)*(x_w - x_e) + 
                     (y_w - y_e)*(y_w - y_e) +   
                     (z_w - z_e)*(z_w - z_e));
                
            x_d = (x_w - x_e)/L;
            y_d = (y_w - y_e)/L;
            z_d = (z_w - z_e)/L;
            VECTOR vector_dir = {.x = x_d, .y = y_d, .z = z_d};
            //pixel = de_que_color(x_d, y_d, z_d);
            pixel = de_que_color(vector_ojo, vector_dir);
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
/**
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
*/

COLOR de_que_color(VECTOR ojo, VECTOR direccion){
  COLOR color;
  VECTOR L;
  intersection* inter = F_inter(ojo, direccion);
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
/**
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
*/

intersection* F_inter(VECTOR a, VECTOR d){
  intersection* inter;
  intersection* near;
  //inter.existe = false;
  inter = NULL;
  near = NULL;
  long double tmin = 100000000000;// = 1/0;
  for (int i = 0; i < lista_length; i++){ 
    inter = calcInterEsfera(listaObjetos[i], a, d); 
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
/**
intersection* calcInterEsfera(sphere* esfera, long double x_d, long double y_d, long double z_d){
    long double b, c, t, t1, t2, discriminante;
    intersection* inter;
    inter = NULL;
    b = 2*( x_d*(x_e - esfera->x_c) + y_d*(y_e - esfera->y_c) + z_d*(z_e - esfera->z_c));
    c = powl((x_e - esfera->x_c), 2) + powl((y_e - esfera->y_c), 2) + powl((z_e - esfera->z_c), 2) - powl(esfera->r, 2);
    discriminante = b*b - 4*c;

    if (discriminante < 0){
        return inter;
    }
    else if (discriminante == 0.0){
        t = -b / 2;
    } else {
        t1 = (-b + sqrtl(discriminante))/2;
        t2 = (-b - sqrtl(discriminante))/2;
        if (t1 < t2 && t1 > 0){
            t = t1;
        }
        else if (t1 < 0 && t2 < 0){
            return inter;
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
*/
intersection* calcInterEsfera(sphere* esfera, VECTOR eye, VECTOR d){
    long double b, c, t, t1, t2, discriminante;
    intersection* inter;
    inter = NULL;
    b = 2*( d.x*(eye.x - esfera->x_c) + d.y*(eye.y - esfera->y_c) + d.z*(eye.z - esfera->z_c));
    c = powl((eye.x - esfera->x_c), 2) + powl((eye.y - esfera->y_c), 2) + powl((eye.z - esfera->z_c), 2) - powl(esfera->r, 2);
    discriminante = b*b - 4*c;

    if (discriminante < 0){
        return inter;
    }
    else if (discriminante == 0.0){
        t = -b / 2;
    } else {
        t1 = (-b + sqrtl(discriminante))/2;
        t2 = (-b - sqrtl(discriminante))/2;
        if (t1 < t2 && t1 > 0){
            t = t1;
        }
        else if (t1 < 0 && t2 < 0){
            return inter;
        }
        else{
            t = t2;
        }
    }
    inter = malloc(sizeof(intersection));
    inter->punto.x = x_e + t*d.x;
    inter->punto.y = y_e + t*d.y;
    inter->punto.z = z_e + t*d.z;
    
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
    
    char *filename = "figuras.txt";
    FILE *fp = fopen(filename, "r");
    if (fp == NULL){
        printf("Error: could not open file %s", filename);
        exit(1);
    }

    const unsigned MAX_LENGTH = 256;
    char buffer [MAX_LENGTH];
    char* color_buffer;
    char* color_delim = ",";
    char* delim = ";";
    char* key;
    char* value;
    int real_object_size = 0;
    listaObjetos[real_object_size] = malloc(sizeof(sphere));
    while (fgets(buffer, MAX_LENGTH, fp)){ // read each line
        
        key = strtok(buffer, delim);
        value = strtok(NULL, delim);
        if (strcasecmp(key, "x") == 0) {
            listaObjetos[real_object_size]->x_c = atoi(value);
        } else if (strcasecmp(key, "y") == 0){
            listaObjetos[real_object_size]->y_c = atoi(value);
        } else if (strcasecmp(key, "z") == 0){
            listaObjetos[real_object_size]->z_c = atoi(value);
        } else if (strcasecmp(key, "r") == 0){
            listaObjetos[real_object_size]->r = atoi(value);
        } else if (strcasecmp(key, "color") == 0){
            color_buffer = strtok(value, color_delim);
            listaObjetos[real_object_size]->color.r = atoi(color_buffer);
            color_buffer = strtok(NULL, color_delim);
            listaObjetos[real_object_size]->color.g = atoi(color_buffer);
            color_buffer = strtok(NULL, color_delim);
            listaObjetos[real_object_size]->color.b = atoi(color_buffer);     
        } 
 
        if (value == NULL){ // Debe tener un enter para calcular todo
            real_object_size++;
            listaObjetos[real_object_size] = malloc(sizeof(sphere));
        };
    }
    lista_length = real_object_size;

    fclose(fp);
    
    raytracing();
    createImage();
    printf("Todo está sirviendo :)\n");
    return 0;
}

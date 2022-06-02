#include "raytracer.h"

COLOR **buffer;
COLOR background = {.r=0, .g=0, .b=0};

// Producto punto de dos vectores
long double producto_punto(VECTOR a, VECTOR b){
    return a.x*b.x + a.y*b.y + a.z*b.z;
};

// Crea la imagen en base al buffer dando un .avs y un .jpg
void createImage(){
    size_t x, 
    row_width,
    width = W_RES,
    height = H_RES;
    int y;

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
    for (y = H_RES; y >= 0; y--) {
        row = PixelGetNextIteratorRow(iter, &row_width);
        for (x = 0; x < row_width; ++ x) {
            //if (buffer[y][x].r > 0 || buffer[y][x].g > 0 || buffer[y][x].b > 0) printf("Red: %f, Green: %f, Blue: %f \n", buffer[y][x].r, buffer[y][x].g, buffer[y][x].b);
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
            //if (pixel.r > 0 || pixel.g > 0 || pixel.b > 0) printf("Red: %f, Green: %f, Blue: %f \n", pixel.r, pixel.g, pixel.b);
            buffer[j][i].r = pixel.r;
            buffer[j][i].g = pixel.g;
            buffer[j][i].b = pixel.b;
            //if (buffer[j][i].r > 0 || buffer[j][i].g > 0 || buffer[j][i].b > 0) printf("Red: %f, Green: %f, Blue: %f \n", buffer[j][i].r, buffer[j][i].g, buffer[j][i].b);
        }
    }
};

/*
#########################################################################################################################################
-----------------------------------------------------------------------------------------------------------------------------------------
#########################################################################################################################################
*/

COLOR de_que_color(VECTOR ojo, VECTOR direccion){
  COLOR color;
  VECTOR L, R, V;
  long double I = 0;
  long double E = 0;
  long double distance, fatt;
  intersection* inter = F_inter(ojo, direccion);
  if (!inter){
      color = background;
  }
  else{
        V.x = -1 * direccion.x;
        V.y = -1 * direccion.y;
        V.z = -1 * direccion.z;

        for(int j = 0; j < luces_length; j++){
          L.x = listaLuces[j]->x - inter->normal.x;
          L.y = listaLuces[j]->y - inter->normal.y;
          L.z = listaLuces[j]->z - inter->normal.z;
          distance = sqrtl((L.x*L.x)+(L.y*L.y)+(L.z*L.z));
          L.x = L.x/distance;
          L.y = L.y/distance;
          L.z = L.z/distance;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
          fatt = 1/(listaLuces[j]->c1 + listaLuces[j]->c2*distance + listaLuces[j]->c3*distance*distance);
          if (fatt > 1) fatt = 1;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
          if (producto_punto(inter->normal, L) > 0.0 ){
            intersection* obstaculo = F_inter(inter->punto, L);
            if (!obstaculo || obstaculo->t > distance){
              I += ((producto_punto(inter->normal, L)) * listaLuces[j]->I_p * inter->K_D * fatt);
              // Reflexion especular
              R.x = 2* ( inter->normal.x )* producto_punto(inter->normal,L) - L.x;
              R.y = 2* ( inter->normal.y )* producto_punto(inter->normal,L) - L.y;
              R.z = 2* ( inter->normal.z )* producto_punto(inter->normal,L) - L.z;
              if (producto_punto(V, R) > 0){
                  if (inter->figura->circulo){
                    E += powl(producto_punto(V, R), inter->figura->circulo->K_N) * inter->figura->circulo->K_S * listaLuces[j]->I_p * fatt ;
                  } else if (inter->figura->poligono){
                    //printf("ENtro en reflexion especular jfbanpsibfapigbapgbapigbpaisgbpasbgpiabgpiasbgpiabgpi\n");
                    E += powl(producto_punto(V, R), inter->figura->poligono->K_N) * inter->figura->poligono->K_S * listaLuces[j]->I_p * fatt ;
                    continue;
                  }
              }
            }
            if (obstaculo) free(obstaculo);
          } 
        }

      I += I_A * inter->K_A;

      if (I > 1) I = 1;
      else if (I < 0) I = 0;

      if (E > 1) E = 1;
      else if (E < 0) E = 0;

      color.r = I * (inter->color.r/255);
      color.g = I * (inter->color.g/255);
      color.b = I * (inter->color.b/255);

      color.r += E * (1.0 - color.r);
      color.g += E * (1.0 - color.g);
      color.b += E * (1.0 - color.b); 
      free(inter);
  }
  return color;
};

intersection* F_inter(VECTOR a, VECTOR d){
  intersection* inter;
  intersection* near;
  //inter.existe = false;
  inter = NULL;
  near = NULL;
  long double tmin = 100000000000;// = 1/0;
  for (int i = 0; i < figuras_length; i++){ 
    // Revisa si es un circulo o poligono, llama a su interseccion respectivo
    if (listaFiguras[i]->circulo){
        inter = calcInterEsfera(listaFiguras[i]->circulo, a, d); // calcular interseccion de esferas
    } else if (listaFiguras[i]->poligono){
        inter = calcInterPoligono(listaFiguras[i]->poligono, a, d); // calcular interseccion de poligonos
    }
    if (inter && inter->t < tmin && inter->t > EPSILON){
      tmin = inter->t;// tmin = d a inter;
      near = inter;//inter = inter con obj;
      near->figura = listaFiguras[i];
    }
  }
  return near;
};

/*
#########################################################################################################################################
-----------------------------------------------------------------------------------------------------------------------------------------
#########################################################################################################################################
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
    inter->K_D = esfera->K_D;
    inter->K_A = esfera->K_A;
    return inter;
};

intersection* calcInterPoligono(porygon* poligono, VECTOR eye, VECTOR d){
    long double b, c, t, denom;

    intersection* inter;
    inter = NULL;
    
    VECTOR coef_polygons = {.x = poligono->A, .y = poligono->B, .z = poligono->C};
    // comprobar si denominador es 0; es perpendicular
    denom = producto_punto(coef_polygons, d);
    if (denom == 0){
        return inter;
    }    

    t = -1 * (poligono->A*eye.x + poligono->B*eye.y + poligono->C*eye.z + poligono->D ) / denom;

    VECTOR punto_plano = {.x = eye.x + t*d.x, .y = eye.y + t*d.y, .z = eye.z + t*d.z };

    // Ver si el punto esta en el poligono
    bool enPoligono = calcularPuntoEnPoligono(poligono, punto_plano);

    if (enPoligono){
        inter = malloc(sizeof(intersection));
        inter->punto.x = punto_plano.x;
        inter->punto.y = punto_plano.y;
        inter->punto.z = punto_plano.z;
        
        inter->color = poligono->color;
        inter->t = t;
        inter->normal.x = poligono->A;
        inter->normal.y = poligono->B;
        inter->normal.z = poligono->C;
        inter->K_D = poligono->K_D;
        inter->K_A = poligono->K_A;
    } 

    return inter;
};

bool calcularPuntoEnPoligono(porygon* poligono, VECTOR punto){
    int cont_lineas = 0;
    int i;
    char max_coef;

    // Aplastar poligono
    if ( fabsl(poligono->A) >= fabsl(poligono->B) && fabsl(poligono->A) >= fabsl(poligono->C)) max_coef = 'A';
    else if (fabsl(poligono->B) >= fabsl(poligono->C)) max_coef = 'B';
    else max_coef = 'C';

    two_point puntos[poligono->puntos_size];

    // Eliminar un eje y traslada a origen
    for (i = 0; i < poligono->puntos_size; i++){
        switch (max_coef) {
            case 'A': // elimina Xs
                puntos[i].u = poligono->puntos[i].y - punto.y;
                puntos[i].v = poligono->puntos[i].z - punto.z;
                break;
            case 'B': // elimina Ys
                puntos[i].u = poligono->puntos[i].x - punto.x;
                puntos[i].v = poligono->puntos[i].z - punto.z;
                break;
            case 'C': // elimina Zs
                puntos[i].u = poligono->puntos[i].x - punto.x;
                puntos[i].v = poligono->puntos[i].y - punto.y;
                break;
            default:
                break;
        }
    };

    // contar paredes
    for (i = 0; i < (poligono->puntos_size)-1; i++){
        if (contar_pared(puntos[i], puntos[i+1])){
            cont_lineas++;
        }
    }
    if (contar_pared(puntos[(poligono->puntos_size)-1], puntos[0])) cont_lineas++;

    return (cont_lineas % 2 == 1);
}

bool contar_pared(two_point punto1, two_point punto2){
    long double m, b;
    if ( (punto1.u < 0 && punto2.u < 0) ||
         (punto1.v > 0 && punto2.v > 0) ||
         (punto1.v < 0 && punto2.v < 0)) { //Rechazo trivial
        return false;

    } else if ((punto1.u > 0 && punto2.u > 0) && (punto1.v > 0 && punto2.v < 0) ||
               (punto1.u > 0 && punto2.u > 0) && (punto1.v < 0 && punto2.v > 0)) { // Aceptacion trivial
        return true;
    }
    else { 
        m = (punto2.v - punto1.v) / (punto2.u - punto1.u); // caso complicado
        b = punto1.v - m*punto1.u;
        if ( (-1*b)/m >= 0){ // si x son positivas
            return true;
        }
        return false; 
    }
}

/*
#########################################################################################################################################
-----------------------------------------------------------------------------------------------------------------------------------------
#########################################################################################################################################
*/

void loadFiguras(){
    char *filename = "esferas.txt";
    const unsigned MAX_LENGTH = 256;

    FILE *fp = fopen(filename, "r");
    if (fp == NULL){
        printf("Error: could not open file %s", filename);
        exit(1);
    }

    char buffer [MAX_LENGTH];
    char* color_buffer;
    char* color_delim = ",";
    char* delim = ";";
    char* key;
    char* value;

    int real_object_size = 0;

    listaEsferas[real_object_size] = malloc(sizeof(sphere));
    while (fgets(buffer, MAX_LENGTH, fp)){ // read each line
        key = strtok(buffer, delim);
        value = strtok(NULL, delim);
        if (strcasecmp(key, "x") == 0) {
            listaEsferas[real_object_size]->x_c = atoi(value);
        } else if (strcasecmp(key, "y") == 0){
            listaEsferas[real_object_size]->y_c = atoi(value);
        } else if (strcasecmp(key, "z") == 0){
            listaEsferas[real_object_size]->z_c = atoi(value);
        } else if (strcasecmp(key, "r") == 0){
            listaEsferas[real_object_size]->r = atoi(value);
        } else if (strcasecmp(key, "K_D") == 0){
            listaEsferas[real_object_size]->K_D = strtold(value, NULL);
        } else if (strcasecmp(key, "K_A") == 0){
            listaEsferas[real_object_size]->K_A = strtold(value, NULL);
        } else if (strcasecmp(key, "K_S") == 0){
            listaEsferas[real_object_size]->K_S = strtold(value, NULL);
        } else if (strcasecmp(key, "K_N") == 0){
            listaEsferas[real_object_size]->K_N = strtold(value, NULL);
        } else if (strcasecmp(key, "color") == 0){
            color_buffer = strtok(value, color_delim);
            listaEsferas[real_object_size]->color.r = atoi(color_buffer);
            color_buffer = strtok(NULL, color_delim);
            listaEsferas[real_object_size]->color.g = atoi(color_buffer);
            color_buffer = strtok(NULL, color_delim);
            listaEsferas[real_object_size]->color.b = atoi(color_buffer);     
        } 
        if (value == NULL){ // Debe tener un enter para calcular todo
            real_object_size++;
            listaEsferas[real_object_size] = malloc(sizeof(sphere));
            listaFiguras[figuras_length] = malloc(sizeof(figura));
            listaFiguras[figuras_length]->circulo = listaEsferas[real_object_size - 1];
            listaFiguras[figuras_length++]->poligono = NULL;
        };
    }

    fclose(fp);

};

void loadLuces(){
    char *lucesFile = "luces.txt";
    const unsigned MAX_LENGTH = 256;
    FILE *fp1 = fopen(lucesFile, "r");
    if (fp1 == NULL){
        printf("Error: could not open file %s", lucesFile);
        exit(1);
    }

    char buffer [MAX_LENGTH];
    char* color_buffer;
    char* color_delim = ",";
    char* delim = ";";
    char* key;
    char* value;

    int real_light_size = 0;

    listaLuces[real_light_size] = malloc(sizeof(light));
    while (fgets(buffer, MAX_LENGTH, fp1)){ // read each line
        
        key = strtok(buffer, delim);
        value = strtok(NULL, delim);
        if (strcasecmp(key, "x") == 0) {
            listaLuces[real_light_size]->x = atoi(value);
        } else if (strcasecmp(key, "y") == 0){
            listaLuces[real_light_size]->y = atoi(value);
        } else if (strcasecmp(key, "z") == 0){
            listaLuces[real_light_size]->z = atoi(value);
        } else if (strcasecmp(key, "I_p") == 0){
            listaLuces[real_light_size]->I_p = strtold(value, NULL);
        } else if (strcasecmp(key, "c1") == 0){
            listaLuces[real_light_size]->c1 = strtold(value, NULL);
        } else if (strcasecmp(key, "c2") == 0){
            listaLuces[real_light_size]->c2 = strtold(value, NULL);
        } else if (strcasecmp(key, "c3") == 0){
            listaLuces[real_light_size]->c3 = strtold(value, NULL);
        }
 
        if (value == NULL){ // Debe tener un enter para calcular todo
            real_light_size++;
            listaLuces[real_light_size] = malloc(sizeof(light));
        };
    }
    luces_length = real_light_size;

    fclose(fp1);
};

void loadPoligonos(){
    char *porygonFile = "poligonos.txt";
    const unsigned MAX_LENGTH = 256;
    FILE *fp = fopen(porygonFile, "r");
    if (fp == NULL){
        printf("Error: could not open file %s", porygonFile);
        exit(1);
    }

    char buffer [MAX_LENGTH];
    char* color_buffer;
    char* point_buffer;
    char* color_delim = ",";
    char* delim = ";";
    char* key;
    char* value;

    int real_porygon_size = 0;

    listaPoligonos[real_porygon_size] = malloc(sizeof(porygon));
    //listaPoligonos[real_porygon_size]->puntos = point[esferas_length];
    while (fgets(buffer, MAX_LENGTH, fp)){ // read each line
        
        key = strtok(buffer, delim);
        value = strtok(NULL, delim);
        if (strcasecmp(key, "x") == 0) {
            point_buffer = strtok(value, color_delim);
            int cont = 0;
            for (int i = 0; point_buffer != NULL; i++){
                listaPoligonos[real_porygon_size]->puntos[i].x = strtold(point_buffer, NULL);
                point_buffer = strtok(NULL, color_delim);
                cont++;
            }
            listaPoligonos[real_porygon_size]->puntos_size = cont;
        } else if (strcasecmp(key, "y") == 0){
            point_buffer = strtok(value, color_delim);
            for (int i = 0; point_buffer != NULL; i++){
                listaPoligonos[real_porygon_size]->puntos[i].y = strtold(point_buffer, NULL);
                point_buffer = strtok(NULL, color_delim);
            }
        } else if (strcasecmp(key, "z") == 0){
            point_buffer = strtok(value, color_delim);
            for (int i = 0; point_buffer != NULL; i++){
                listaPoligonos[real_porygon_size]->puntos[i].z = strtold(point_buffer, NULL);
                point_buffer = strtok(NULL, color_delim);
            }

        } else if (strcasecmp(key, "K_D") == 0){
            listaPoligonos[real_porygon_size]->K_D = strtold(value, NULL);
        } else if (strcasecmp(key, "K_A") == 0){
            listaPoligonos[real_porygon_size]->K_A = strtold(value, NULL);
        } else if (strcasecmp(key, "K_S") == 0){
            listaPoligonos[real_porygon_size]->K_S = strtold(value, NULL);
        } else if (strcasecmp(key, "K_N") == 0){
            listaPoligonos[real_porygon_size]->K_N = strtold(value, NULL);
        } else if (strcasecmp(key, "color") == 0){
            color_buffer = strtok(value, color_delim);
            listaPoligonos[real_porygon_size]->color.r = atoi(color_buffer);
            color_buffer = strtok(NULL, color_delim);
            listaPoligonos[real_porygon_size]->color.g = atoi(color_buffer);
            color_buffer = strtok(NULL, color_delim);
            listaPoligonos[real_porygon_size]->color.b = atoi(color_buffer);     
        } 
 
        if (value == NULL){ // Debe tener un enter para calcular todo
            point vertex0 = listaPoligonos[real_porygon_size]->puntos[0];
            point vertex1 = listaPoligonos[real_porygon_size]->puntos[1];
            point vertex2 = listaPoligonos[real_porygon_size]->puntos[2];

            VECTOR vector1 = {.x = vertex1.x - vertex0.x , .y = vertex1.y - vertex0.y, .z = vertex1.z - vertex0.z };
            VECTOR vector2 = {.x = vertex2.x - vertex0.x , .y = vertex2.y - vertex0.y, .z = vertex2.z - vertex0.z };
            
            //printf("v1.x = %Lf, v1.y = %Lf, v1.z = %Lf\n", vector1.x, vector1.y, vector1.z);
            //printf("v2.x = %Lf, v2.y = %Lf, v2.z = %Lf\n", vector2.x, vector2.y, vector2.z);
            
            listaPoligonos[real_porygon_size]->A = (vector1.y * vector2.z) - (vector1.z * vector2.y);
            listaPoligonos[real_porygon_size]->B = (vector1.z * vector2.x) - (vector1.x * vector2.z);
            listaPoligonos[real_porygon_size]->C = (vector1.x * vector2.y) - (vector1.y * vector2.x);
            listaPoligonos[real_porygon_size]->D = -1 * (listaPoligonos[real_porygon_size]->A * vertex0.x + 
                                                         listaPoligonos[real_porygon_size]->B * vertex0.y +
                                                         listaPoligonos[real_porygon_size]->C * vertex0.z  );
                                                         
            //printf("A: %Lf, B: %Lf, C: %Lf, D: %Lf\n", listaPoligonos[0]->A, listaPoligonos[0]->B, listaPoligonos[0]->C, listaPoligonos[0]->D);

            long double norma = sqrtl(listaPoligonos[real_porygon_size]->A*listaPoligonos[real_porygon_size]->A + 
                                      listaPoligonos[real_porygon_size]->B*listaPoligonos[real_porygon_size]->B + 
                                      listaPoligonos[real_porygon_size]->C*listaPoligonos[real_porygon_size]->C);
            // Normalizar
            listaPoligonos[real_porygon_size]->A = listaPoligonos[real_porygon_size]->A / norma;
            listaPoligonos[real_porygon_size]->B = listaPoligonos[real_porygon_size]->B / norma;
            listaPoligonos[real_porygon_size]->C = listaPoligonos[real_porygon_size]->C / norma;
            listaPoligonos[real_porygon_size]->D = listaPoligonos[real_porygon_size]->D / norma;

            real_porygon_size++;
            listaPoligonos[real_porygon_size] = malloc(sizeof(porygon));

            listaFiguras[figuras_length] = malloc(sizeof(figura));
            listaFiguras[figuras_length]->poligono = listaPoligonos[real_porygon_size - 1];
            listaFiguras[figuras_length++]->circulo = NULL;
        };
    }

    fclose(fp);
};

int main (){
    int i, j;
    // crear el buffer
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

    // Carga los archivos de las figuras y luces
    loadFiguras();
    loadLuces();
    loadPoligonos();
    
    raytracing();
    createImage();
    printf("Todo está sirviendo :)\n");
    return 0;
}

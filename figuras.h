typedef struct {
  double r;
  double g;
  double b;
} COLOR;

typedef struct {
  long double x;
  long double y;
  long double z;
} point;

typedef struct {
  long double x_c;
  long double y_c;
  long double z_c;
  long double r;
  // iluminacion
  long double K_D; // coeficiente reflexion difusa (material)
  long double K_A; // ambiente
  // reflexion difusa
  long double K_S; // material
  long double K_N; // mancha blanca
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

typedef struct {
  long double x;
  long double y;
  long double z;
  long double I_p;
  long double c1;
  long double c2;
  long double c3;
} light;

typedef struct {
  point puntos[30];
  int puntos_size;
  long double A;
  long double B;
  long double C;
  //long double D;
  // iluminacion
  long double K_D; // coeficiente reflexion difusa (material)
  long double K_A; // ambiente
  // reflexion difusa
  long double K_S; // material
  long double K_N; // mancha blanca
  COLOR color;
} porygon;

typedef struct {
  sphere* circulo;
  porygon* poligono;
} figura;
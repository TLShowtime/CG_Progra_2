# CG-Proyecto 2
Proyecto programado 2 de Introducción a los gráficos por computadora, semestre 1 2022

Para compilar el proyecto se usa el siguiente comando:
```
make all
```
Para limpiar los archivos compilados se usa el siguiente comando:
```
make clean
```

Para ejecutar el proyecto se usa la siguiente sintaxis:
```
make run 
```
o
```
./raytracer
```

Se crean dos imagenes, un .avs y un .jpg

Se usa dos archivo de figuras (para tener mejor orden) llamados **esferas.txt** y **poligonos.txt** donde la sintaxis del primero es la siguiente:

```
    x;value
    y;value
    z;value
    r;value
    K_D;value // valor entre 0-1
    K_A;value // valor entre 0-1
    K_S;value // valor entre 0-1
    K_N;value // valor > 0
    color;r_value,g_value,b_value

```
y el segundo tiene la siguiente sintaxis:

```
    x;value, value, ...
    y;value, value, ...
    z;value, value, ...
    K_D;value // valor entre 0-1
    K_A;value // valor entre 0-1
    K_S;value // valor entre 0-1
    K_N;value // valor > 0
    color;r_value,g_value,b_value

```


Cada figura debe estar separada por un ENTER. El archivo debe terminar con un ENTER para registrar la ultima figura. Se supone que la sintaxis es valida.

Se usa un archivo de luces llamado **luces.txt** donde la sintaxis es la siguiente:

```
    x;value
    y;value
    z;value
    I_p;value // valor entre 0-1
    c1;value // valor entre 0-1
    c2;value // valor entre 0-1
    c3;value // valor entre 0-1

```
Cada luz debe estar separada por un ENTER. El archivo debe terminar con un ENTER para registrar la ultima luz. Se supone que la sintaxis es valida.
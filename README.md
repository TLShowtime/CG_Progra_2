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

Se usa un archivo de figuras llamado **figuras.txt** donde la sintaxis es el siguiente:

```
    x;value
    y;value
    z;value
    r;value
    color;r_value,g_value,b_value

```
El archivo debe terminar con un enter para registrar la ultima figura. Se supone que la sintaxis es valida.
#include "raytracer.h"

COLOR **buffer;

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
    MagickWriteImage(wand, "imagen.jpeg");

    // Termina la libreria
    wand = DestroyMagickWand(wand);
    MagickWandTerminus();
};

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
    
    createImage();
    return 0;
}
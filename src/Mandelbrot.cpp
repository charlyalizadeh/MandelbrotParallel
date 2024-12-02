#include "../include/Mandelbrot.hpp"


#include <cstdio>
int computeMandelbrotPixel(int pixelX, int pixelY,
                           int originX, int originY,
                           float pixelDimensionX, float pixelDimensionY,
                           int precision)
{
    float cR, cI, zR, zI;
    float temp;
    int i;

    cR = (pixelX - originX) * pixelDimensionX;
    cI = (pixelY - originY) * pixelDimensionY;
    zR = cR;
    zI = cI;
    i = 0;
    while(i < precision && zR * zR + zI * zI <= 4)
    {
        temp = zR;
        zR = zR * zR - zI * zI + cR;
        zI = 2 * temp * zI + cI;
        i++;
    }
    return i;
}

void colorPixel(unsigned char* pixelColors,
                int index,
                int r, int g, int b, int a)
{
    pixelColors[index] = r;
    pixelColors[index + 1] = g;
    pixelColors[index + 2] = b;
    pixelColors[index + 3] = a;
}

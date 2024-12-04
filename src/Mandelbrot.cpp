#include "../include/Mandelbrot.hpp"


int computeMandelbrotPixel(int pixelX, int pixelY,
                           int originX, int originY,
                           double pixelDimensionX, double pixelDimensionY,
                           int precision)
{
    double cR, cI, zR, zI;
    double temp;
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

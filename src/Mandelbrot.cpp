#include "../include/Mandelbrot.hpp"


int computeMandelbrotPixel(int pixelX, int pixelY,
                           int originX, int originY,
                           float pixelDimensionX, float pixelDimensionY,
                           int precision)
{
    float cX, cY, zX, zY;
    float temp;
    int i;

    cX = (pixelX - originX) * pixelDimensionX;
    cY = (originY - pixelY) * pixelDimensionY;
    zX = cX;
    zY = cY;
    i = 0;
    while(i < precision && zX * zX + zY * zY <= 4)
    {
        temp = zY;
        zY = 2 * zX * zY + cY;
        zX = zX * zX - temp * temp + cX;
        i++;
    }
    return i;
}

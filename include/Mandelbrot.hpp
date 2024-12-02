#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <SFML/System/Vector2.hpp>


int computeMandelbrotPixel(int pixelX, int pixelY,
                           int originX, int originY,
                           float pixelDimensionX, float pixelDimensionY,
                           int precision);

void colorPixel(unsigned char* pixelColors,
                int index,
                int r, int g, int b, int a);


#endif

#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <SFML/System/Vector2.hpp>


int computeMandelbrotPixel(int pixelX, int pixelY,
                           int originX, int originY,
                           float pixelDimensionX, float pixelDimensionY,
                           int precision);


#endif

#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <SFML/System/Vector2.hpp>


int computeMandelbrotPixel(int pixelX, int pixelY,
                           int originX, int originY,
                           double pixelDimensionX, double pixelDimensionY,
                           int precision);



#endif

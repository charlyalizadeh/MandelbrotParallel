#ifndef MANDELBROTSINGLETHREAD_H
#define MANDELBROTSINGLETHREAD_H

#include <SFML/System/Vector2.hpp>
#include "VectorUtility.hpp"
#include "Mandelbrot.hpp"


void computeMandelbrotSingleThread(const sf::Vector2i& origin,
                                   const sf::Vector2i& dimension,
                                   const sf::Vector2f& pixelDimensions,
                                   int precision,
                                   unsigned char *pixelColors);


#endif

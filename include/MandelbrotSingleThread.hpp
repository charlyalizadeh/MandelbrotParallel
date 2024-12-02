#ifndef MANDELBROTSINGLETHREAD_H
#define MANDELBROTSINGLETHREAD_H

#include <SFML/System/Vector2.hpp>
#include <immintrin.h>
#include "Mandelbrot.hpp"


void computeMandelbrotSingleThread(const sf::Vector2i& origin,
                                   const sf::Vector2i& dimension,
                                   const sf::Vector2<double>& pixelDimensions,
                                   int precision,
                                   unsigned char *pixelColors);

void computeMandelbrotSingleThreadIntrinsic(const sf::Vector2i& origin,
                                            const sf::Vector2i& dimension,
                                            const sf::Vector2<double>& pixelDimensions,
                                            int precision,
                                            unsigned char *pixelColors);


#endif

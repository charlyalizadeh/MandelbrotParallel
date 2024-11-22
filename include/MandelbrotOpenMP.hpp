#ifndef MANDELBROTOPENMP_H
#define MANDELBROTOPENMP_H

#include <SFML/System/Vector2.hpp>
#include <omp.h>
#include "VectorUtility.hpp"
#include "Mandelbrot.hpp"


void computeMandelbrotOpenMP(const sf::Vector2i& origin,
                             const sf::Vector2i& dimension,
                             const sf::Vector2f& pixelDimensions,
                             int precision,
                             unsigned char *pixelColors,
                             unsigned int numThread);


#endif

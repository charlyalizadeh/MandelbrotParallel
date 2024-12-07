#ifndef MANDELBROTOPENMP_H
#define MANDELBROTOPENMP_H

#include <SFML/System/Vector2.hpp>
#include <omp.h>
#include <immintrin.h>
#include "Mandelbrot.hpp"


void computeMandelbrotOpenMP(const sf::Vector2i& origin,
                             const sf::Vector2i& dimension,
                             const sf::Vector2<double>& pixelDimensions,
                             int precision,
                             unsigned int *pixelIteration,
                             unsigned int numThread);


#endif

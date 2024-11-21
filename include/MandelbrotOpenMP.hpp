#ifndef MANDELBROTOPENMP_H
#define MANDELBROTOPENMP_H

#include <SFML/System/Vector2.hpp>
#include "VectorUtility.hpp"
#include <omp.h>


void computeMandelbrotOpenMP(const sf::Vector2i& origin,
                             const sf::Vector2i& dimension,
                             const sf::Vector2f& pixelDimensions,
                             int precision,
                             unsigned char *pixelColors,
                             unsigned int numThread);


#endif

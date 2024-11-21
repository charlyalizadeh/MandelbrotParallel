#ifndef MANDELBROTTHREAD_H
#define MANDELBROTTHREAD_H

#include <SFML/System/Vector2.hpp>
#include "VectorUtility.hpp"
#include <vector>
#include <thread>


void computeMandelbrotThread(const sf::Vector2i& origin,
                             const sf::Vector2i& dimension,
                             const sf::Vector2f& pixelDimensions,
                             int precision,
                             unsigned char *pixelColors,
                             unsigned int numThread);


#endif

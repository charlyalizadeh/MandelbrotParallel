#ifndef MANDELBROTTHREAD_H
#define MANDELBROTTHREAD_H

#include <vector>
#include <thread>
#include <SFML/System/Vector2.hpp>
#include "Mandelbrot.hpp"


void computeMandelbrotThread(const sf::Vector2i& origin,
                             const sf::Vector2i& dimension,
                             const sf::Vector2<double>& pixelDimensions,
                             int precision,
                             unsigned char *pixelColors,
                             unsigned int numThread);


#endif

#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <SFML/System/Vector2.hpp>
#include "VectorUtility.hpp"


int computeMandelbrotPixel(sf::Vector2i pixel,
                           const sf::Vector2i& origin,
                           const sf::Vector2f& pixelDimensions,
                           int precision);


#endif

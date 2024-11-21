#ifndef VECTORUTILITIES_HPP
#define VECTORUTILITIES_HPP

#include <SFML/System/Vector2.hpp>


sf::Vector2f pixel2coord(sf::Vector2i pixel,
                         sf::Vector2i origin,
                         sf::Vector2f pixelDimensions);

sf::Vector2f complexPow2(sf::Vector2f);


#endif

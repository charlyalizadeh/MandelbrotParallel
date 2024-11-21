#include "../include/VectorUtility.hpp"


sf::Vector2f pixel2coord(sf::Vector2i pixel,
                         sf::Vector2i origin,
                         sf::Vector2f pixelDimensions)
{
    return sf::Vector2f(
                (pixel.x - origin.x) * pixelDimensions.x,
                (origin.y - pixel.y) * pixelDimensions.y
            );
}

sf::Vector2f complexPow2(sf::Vector2f z)
{
    return sf::Vector2f(
               z.x * z.x - z.y * z.y,
               2 * z.x * z.y
           );
}

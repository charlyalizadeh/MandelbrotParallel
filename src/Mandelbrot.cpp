#include "../include/Mandelbrot.hpp"


int computeMandelbrotPixel(sf::Vector2i pixel,
                           const sf::Vector2i& origin,
                           const sf::Vector2f& pixelDimensions,
                           int precision)
{
    sf::Vector2f coord, z;
    int i;

    coord = pixel2coord(pixel, origin, pixelDimensions);
    z = coord;
    i = 0;
    while(i < precision && z.x * z.x + z.y * z.y <= 4)
    {
        z = complexPow2(z) + coord;
        i++;
    }
    return i;
}

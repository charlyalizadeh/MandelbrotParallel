#include "../include/MandelbrotSingleThread.hpp"


static int computeMandelbrotPixel(sf::Vector2i pixel,
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

void computeMandelbrotSingleThread(const sf::Vector2i& origin,
                                   const sf::Vector2i& dimension,
                                   const sf::Vector2f& pixelDimensions,
                                   int precision,
                                   unsigned char *pixelColors)
{
    int mandelbrotValue;
    float ratio;

    for(int x = 0; x < dimension.x; x++)
    {
        for(int y = 0; y < dimension.y; y++)
        {
            mandelbrotValue = computeMandelbrotPixel(sf::Vector2i(x, y), origin, pixelDimensions, precision);
            ratio = (float)mandelbrotValue / precision;
            pixelColors[(y * dimension.x + x) * 4] = ratio * 255;
            pixelColors[(y * dimension.x + x) * 4 + 1] = ratio * 255;
            pixelColors[(y * dimension.x + x) * 4 + 2] = ratio * 255;
            pixelColors[(y * dimension.x + x) * 4 + 3] = 255;
        }
    }
}

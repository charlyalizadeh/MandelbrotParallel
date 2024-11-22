#include "../include/MandelbrotSingleThread.hpp"


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
            mandelbrotValue = computeMandelbrotPixel(x, y, origin.x, origin.y, pixelDimensions.x, pixelDimensions.y, precision);
            ratio = (float)mandelbrotValue / precision * 5;
            if(mandelbrotValue == precision)
            {
                pixelColors[(y * dimension.x + x) * 4] = 0;
                pixelColors[(y * dimension.x + x) * 4 + 1] = 0;
                pixelColors[(y * dimension.x + x) * 4 + 2] = 0;
                pixelColors[(y * dimension.x + x) * 4 + 3] = 0;
            }
            else
            {
                pixelColors[(y * dimension.x + x) * 4] = ratio * 255;
                pixelColors[(y * dimension.x + x) * 4 + 1] = ratio * 255;
                pixelColors[(y * dimension.x + x) * 4 + 2] = (1 - ratio * 255);
                pixelColors[(y * dimension.x + x) * 4 + 3] = 255;
            }
        }
    }
}

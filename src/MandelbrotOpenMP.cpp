#include "../include/MandelbrotOpenMP.hpp"


void computeMandelbrotOpenMP(const sf::Vector2i& origin,
                             const sf::Vector2i& dimension,
                             const sf::Vector2f& pixelDimensions,
                             int precision,
                             unsigned char *pixelColors,
                             unsigned int numThread)
{
    omp_set_num_threads(numThread);
    #pragma omp parallel for
    for(int y = 0; y < dimension.y; y++)
    {
        for(int x = 0; x < dimension.x; x++)
        {
            int mandelbrotValue = computeMandelbrotPixel(x, y, origin.x, origin.y, pixelDimensions.x, pixelDimensions.y, precision);
            float ratio = (float)mandelbrotValue / precision * 5;
            if(mandelbrotValue == precision)
                colorPixel(pixelColors, (y * dimension.x + x) * 4, 255, 255, 255, 255);
            else
                colorPixel(pixelColors, (y * dimension.x + x) * 4, 0, 0, 0, 255);
        }
    }
}

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
    for(int x = 0; x < dimension.x; x++)
    {
        for(int y = 0; y < dimension.y; y++)
        {
            int mandelbrotValue = computeMandelbrotPixel(x, y, origin.x, origin.y, pixelDimensions.x, pixelDimensions.y, precision);
            float ratio = (float)mandelbrotValue / precision * 5;
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

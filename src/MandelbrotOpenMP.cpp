#include "../include/MandelbrotOpenMP.hpp"


void computeMandelbrotOpenMP(const sf::Vector2i& origin,
                             const sf::Vector2i& dimension,
                             const sf::Vector2<double>& pixelDimensions,
                             int precision,
                             unsigned int *pixelIteration,
                             unsigned int numThread)
{
    omp_set_num_threads(numThread);
    #pragma omp parallel for
    for(int y = 0; y < dimension.y; y++)
    {
        for(int x = 0; x < dimension.x; x++)
        {
            int iteration = computeMandelbrotPixel(x, y, origin.x, origin.y, pixelDimensions.x, pixelDimensions.y, precision);
            pixelIteration[y * dimension.x + x] = iteration;
        }
    }
}

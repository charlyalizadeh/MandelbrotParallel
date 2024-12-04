#include "../include/MandelbrotThread.hpp"


static void computeMandelbrotRow(const sf::Vector2i& origin,
                                 const sf::Vector2i& dimension,
                                 const sf::Vector2<double>& pixelDimensions,
                                 int precision,
                                 unsigned int *pixelIteration,
                                 int x)
{
    for(int y = 0; y < dimension.y; y++)
    {
        int iteration = computeMandelbrotPixel(x, y, origin.x, origin.y, pixelDimensions.x, pixelDimensions.y, precision);
        pixelIteration[y * dimension.x + x] = iteration;
    }
}

static void computeMandelbrotRows(const sf::Vector2i& origin,
                                  const sf::Vector2i& dimension,
                                  const sf::Vector2<double>& pixelDimensions,
                                  int precision,
                                  unsigned int *pixelIteration,
                                  int xStart, int xEnd)
{
    for(int x = xStart; x < xEnd; x++)
        computeMandelbrotRow(origin, dimension, pixelDimensions, precision, pixelIteration, x);
}

void computeMandelbrotThread(const sf::Vector2i& origin,
                             const sf::Vector2i& dimension,
                             const sf::Vector2<double>& pixelDimensions,
                             int precision,
                             unsigned int *pixelIteration,
                             unsigned int numThread)
{
    int nRowPerThread = dimension.x / numThread;
    std::vector<std::thread> threads;
    int iThread = 0;
    int xEnd;
    for(int x = 0; x < dimension.x; x += nRowPerThread)
    {
        xEnd = x + nRowPerThread;
        xEnd = xEnd > dimension.x ? dimension.x : xEnd;
        threads.push_back(std::thread(computeMandelbrotRows,
                                      origin,
                                      dimension,
                                      pixelDimensions,
                                      precision,
                                      pixelIteration,
                                      x, xEnd));
        iThread++;
    }
    for(int i = 0; i < threads.size(); i++)
        threads[i].join();
}

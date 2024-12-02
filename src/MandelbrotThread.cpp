#include "../include/MandelbrotThread.hpp"


static void computeMandelbrotRow(const sf::Vector2i& origin,
                                 const sf::Vector2i& dimension,
                                 const sf::Vector2<double>& pixelDimensions,
                                 int precision,
                                 unsigned char *pixelColors,
                                 int x)
{
    for(int y = 0; y < dimension.y; y++)
    {
        int mandelbrotValue = computeMandelbrotPixel(x, y, origin.x, origin.y, pixelDimensions.x, pixelDimensions.y, precision);
        float ratio = (float)mandelbrotValue / precision * 5;
        if(mandelbrotValue == precision)
            colorPixel(pixelColors, (y * dimension.x + x) * 4, 255, 255, 255, 255);
        else
            colorPixel(pixelColors, (y * dimension.x + x) * 4, 0, 0, 0, 255);
    }
}

static void computeMandelbrotRows(const sf::Vector2i& origin,
                                  const sf::Vector2i& dimension,
                                  const sf::Vector2<double>& pixelDimensions,
                                  int precision,
                                  unsigned char *pixelColors,
                                  int xStart, int xEnd)
{
    for(int x = xStart; x < xEnd; x++)
        computeMandelbrotRow(origin, dimension, pixelDimensions, precision, pixelColors, x);
}

void computeMandelbrotThread(const sf::Vector2i& origin,
                             const sf::Vector2i& dimension,
                             const sf::Vector2<double>& pixelDimensions,
                             int precision,
                             unsigned char *pixelColors,
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
                                      pixelColors,
                                      x, xEnd));
        iThread++;
    }
    for(int i = 0; i < threads.size(); i++)
        threads[i].join();
}

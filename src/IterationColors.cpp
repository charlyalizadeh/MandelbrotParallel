#include "../include/IterationColors.hpp"


void colorPixel(unsigned char* pixelColors,
                int index,
                int r, int g, int b, int a)
{
    pixelColors[index] = r;
    pixelColors[index + 1] = g;
    pixelColors[index + 2] = b;
    pixelColors[index + 3] = a;
}

void iterationToColorsBlackWhite(unsigned int *pixelIteration,
                                 int width, int height,
                                 int maxIteration, unsigned char *pixelColors)
{
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            if(pixelIteration[y * width + x] == maxIteration)
                colorPixel(pixelColors, (y * width + x) * 4, 255, 255, 255, 255);
            else
                colorPixel(pixelColors, (y * width + x) * 4, 0, 0, 0, 255);
        }
    }
}

void iterationToColorsGradient(unsigned int *pixelIteration,
                               int width, int height,
                               int maxIteration, std::vector<sf::Vector3i> gradient, unsigned char *pixelColors)
{
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            if(pixelIteration[y * width + x] == maxIteration)
            {
                colorPixel(pixelColors, (y * width + x) * 4, 0, 0, 0, 255);
            }
            else
            {
                double smoothed = std::log2(std::log2(x * x + y * y) / 2.0);
                int colorI = (int)(std::sqrt(pixelIteration[y * width + x] + 10 - smoothed) * 256) % gradient.size();
                sf::Vector3i color = gradient[colorI];
                colorPixel(pixelColors, (y * width + x) * 4, color.x, color.y, color.z, 255);
            }
        }
    }
}

std::vector<sf::Vector3i> createColorInterpolation(sf::Vector3i color1, sf::Vector3i color2, int n)
{
    std::vector<sf::Vector3i> interpolation;
    float step = 1.0 / (n + 1);
    float t = step;
    for(int i = 0; i < n; i++)
    {
        interpolation.push_back(sf::Vector3i(
                    color1.x + (color2.x - color1.x) * t,
                    color1.y + (color2.y - color1.y) * t,
                    color1.z + (color2.z - color1.z) * t
        ));
        t += step;
    }
    return interpolation;
}

std::vector<sf::Vector3i> createColorGradient(std::vector<sf::Vector3i> colors, std::vector<int> n)
{
    std::vector<sf::Vector3i> gradient;
    int totalN = 0;
    for(int i = 0; i < n.size(); i++)
        totalN += n[i] + colors.size();
    gradient.reserve(totalN);
    for(int i = 0; i < colors.size() - 1; i++)
    {
        std::vector<sf::Vector3i> interpolation = createColorInterpolation(colors[i], colors[i + 1], n[i]);
        gradient.push_back(colors[i]);
        gradient.insert(gradient.end(), interpolation.begin(), interpolation.end());
        if(i == colors.size() - 2)
            gradient.push_back(colors[colors.size() - 1]);
    }
    return gradient;
}

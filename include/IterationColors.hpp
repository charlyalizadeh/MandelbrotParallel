#ifndef ITERATIONCOLORS_H
#define ITERATIONCOLORS_H

#include <vector>
#include <cmath>
#include <SFML/System/Vector3.hpp>

void colorPixel(unsigned char* pixelColors,
                int index,
                int r, int g, int b, int a);

void iterationToColorsBlackWhite(unsigned int *pixelIteration,
                                 int width, int height,
                                 int maxIteration, unsigned char *pixelColors);

void iterationToColorsGradient(unsigned int *pixelIteration,
                               int width, int height,
                               int maxIteration, std::vector<sf::Vector3i> gradient, unsigned char *pixelColors);

std::vector<sf::Vector3i> createColorInterpolation(sf::Vector3i color1,
                                                   sf::Vector3i color2,
                                                   int n);

std::vector<sf::Vector3i> createColorGradient(std::vector<sf::Vector3i> colors,
                                              std::vector<int> n);


#endif

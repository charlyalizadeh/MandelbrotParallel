#include "../include/MandelbrotGUIDrawer.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

int main()
{
    sf::Vector2i dimension(1920, 1080);
    sf::RenderWindow window(sf::VideoMode(dimension.x, dimension.y), "Mandelbrot");
    MandelbrotState initState(dimension,
                              sf::Vector2i(dimension.x / 2, dimension.y / 2),
                              sf::Vector2<double>(0.001, 0.001),
                              300,
                              1,
                              24,
                              true);
    MandelbrotGUIDrawer guiDrawer(initState);
    guiDrawer.start(window);
    return 0;
}

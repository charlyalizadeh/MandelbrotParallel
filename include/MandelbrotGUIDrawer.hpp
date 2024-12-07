#ifndef MANDELBROTGUIDRAWER_H
#define MANDELBROTGUIDRAWER_H

#include <string>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include "../lib/imgui/imgui.h"
#include "../lib/imgui/imgui-SFML.h"
#include "MandelbrotDrawer.hpp"


class MandelbrotGUIDrawer
{
private:
    bool isDraging;
    sf::Vector2i oldMouseCoord;
    sf::Clock deltaClock;
    char savepath[128];
    MandelbrotDrawer drawer;

    void updateSFML(sf::RenderWindow& window);
    void updateImGui(sf::RenderWindow& window);
    void update(sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

public:
    MandelbrotGUIDrawer(const MandelbrotState& initState);
    void start(sf::RenderWindow& window);
};


#endif

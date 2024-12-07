#ifndef MANDELBROTDRAWER_H
#define MANDELBROTDRAWER_H

#include <chrono>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "../include/MandelbrotSingleThread.hpp"
#include "../include/MandelbrotOpenMP.hpp"
#include "../include/MandelbrotThread.hpp"
#include "../include/IterationColors.hpp"


struct MandelbrotState
{
    sf::Vector2i dimension;
    sf::Vector2i origin;
    sf::Vector2<double> pixelDimension;
    int precision;
    int computeMethod;
    int nThread;
    bool intrinsic;

    MandelbrotState();
    MandelbrotState(sf::Vector2i dimension,
                    sf::Vector2i origin,
                    sf::Vector2<double> pixelDimension,
                    int precision,
                    int computeMethod,
                    int nThread,
                    bool intrinsic);
};
bool operator !=(const MandelbrotState& m1, const MandelbrotState& m2);
bool operator ==(const MandelbrotState& m1, const MandelbrotState& m2);

class MandelbrotDrawer
{
private:
    MandelbrotState currentState;
    MandelbrotState bufferState;
    unsigned int* pixelIteration;
    unsigned char* pixelColors;

    double computeTime;
    std::vector<sf::Vector3i> gradient;
    sf::Texture texture;
    sf::Sprite sprite;

    void init();

public:
    void getComputeTime();
    void computeMandelbrot(bool force = false);
    void drawAxes(sf::RenderWindow& window);
    void saveImage(char* savepath);
    void saveVideo(sf::RenderWindow& window, sf::Vector2<double> to, int nbIteration, double zoomDelta);
    void setOrigin(sf::Vector2i origin);
    void zoomToScreen(sf::Vector2i position, float delta, int direction);
    void zoomToCoord(sf::Vector2<double> position, float delta, int direction);

    MandelbrotDrawer(sf::Vector2i dimension,
                     sf::Vector2i origin,
                     sf::Vector2<double> pixelDimension,
                     int precision,
                     int computeMethod,
                     int nThread,
                     bool intrinsic);
    MandelbrotDrawer(const MandelbrotState& initState);
    ~MandelbrotDrawer();

    friend class MandelbrotGUIDrawer;
};


#endif

#include "../include/MandelbrotDrawer.hpp"

using std::chrono::high_resolution_clock;
using std::chrono::duration;

MandelbrotState::MandelbrotState() {}
MandelbrotState::MandelbrotState(sf::Vector2i dimension,
                                 sf::Vector2i origin,
                                 sf::Vector2<double> pixelDimension,
                                 int precision,
                                 int computeMethod,
                                 int nThread,
                                 bool intrinsic) :
                                     dimension(dimension),
                                     origin(origin),
                                     pixelDimension(pixelDimension),
                                     precision(precision),
                                     computeMethod(computeMethod),
                                     nThread(nThread),
                                     intrinsic(intrinsic) {}
bool operator !=(const MandelbrotState& m1, const MandelbrotState& m2)
{
    return (m1.dimension != m2.dimension ||
            m1.origin != m2.origin ||
            m1.pixelDimension != m2.pixelDimension ||
            m1.precision != m2.precision ||
            m1.computeMethod != m2.computeMethod ||
            m1.nThread != m2.nThread ||
            m1.intrinsic != m2.intrinsic);
}
bool operator ==(const MandelbrotState& m1, const MandelbrotState& m2)
{
    return (m1.dimension == m2.dimension &&
            m1.origin == m2.origin &&
            m1.pixelDimension == m2.pixelDimension &&
            m1.precision == m2.precision &&
            m1.computeMethod == m2.computeMethod &&
            m1.nThread == m2.nThread &&
            m1.intrinsic == m2.intrinsic);
}

void MandelbrotDrawer::init()
{
    pixelColors = new sf::Uint8[currentState.dimension.x * currentState.dimension.y * 4];
    pixelIteration = new unsigned int[currentState.dimension.x * currentState.dimension.y];

    std::vector<sf::Vector3i> colors = {
        sf::Vector3i(75, 67, 118),
        sf::Vector3i(174, 68, 90),
        sf::Vector3i(232, 188, 185),
        sf::Vector3i(184, 0, 31),
        sf::Vector3i(232, 188, 185),
        sf::Vector3i(174, 68, 90),
        sf::Vector3i(75, 67, 118)
    };
    std::vector<int> steps = {200, 1000, 1000, 1000, 1000, 200};
    gradient = createColorGradient(colors, steps);

    iterationToColorsGradient(pixelIteration,
                              currentState.dimension.x,
                              currentState.dimension.y,
                              currentState.precision,
                              gradient,
                              pixelColors);
    texture.create(currentState.dimension.x, currentState.dimension.y);
    texture.create(currentState.dimension.x, currentState.dimension.y);
    texture.update(pixelColors);
    sprite.setTexture(texture);
    sprite.setPosition({0, 0});
}
MandelbrotDrawer::MandelbrotDrawer(sf::Vector2i dimension,
                                   sf::Vector2i origin,
                                   sf::Vector2<double> pixelDimension,
                                   int precision,
                                   int computeMethod,
                                   int nThread,
                                   bool intrinsic)
{
    currentState = MandelbrotState(dimension, origin, pixelDimension, precision, computeMethod, nThread, intrinsic);
    bufferState = currentState;
    init();
}
MandelbrotDrawer::MandelbrotDrawer(const MandelbrotState& initState)
{
    currentState = initState;
    bufferState = currentState;
    init();
}
MandelbrotDrawer::~MandelbrotDrawer()
{
    delete[] pixelIteration;
    delete[] pixelColors;
}

void MandelbrotDrawer::setOrigin(sf::Vector2i origin)
{
    bufferState.origin = origin;
}
void MandelbrotDrawer::zoomToScreen(sf::Vector2i coord, float delta, int direction)
{
    sf::Vector2<double> oldCoord = sf::Vector2<double>(coord - bufferState.origin);
    oldCoord.x *= bufferState.pixelDimension.x;
    oldCoord.y *= bufferState.pixelDimension.y;
    bufferState.pixelDimension.x -= direction * delta * bufferState.pixelDimension.x;
    bufferState.pixelDimension.y -= direction * delta * bufferState.pixelDimension.y;
    sf::Vector2<double> newCoord = sf::Vector2<double>(coord - bufferState.origin);
    newCoord.x *= bufferState.pixelDimension.x;
    newCoord.y *= bufferState.pixelDimension.y;
    sf::Vector2<double> offset = newCoord - oldCoord;
    offset.x /= bufferState.pixelDimension.x;
    offset.y /= bufferState.pixelDimension.y;
    bufferState.origin += sf::Vector2i(offset);
}
void MandelbrotDrawer::zoomToCoord(sf::Vector2<double> coord, float delta, int direction)
{
    sf::Vector2i screenCoord;

    screenCoord.x = coord.x / bufferState.pixelDimension.x + bufferState.origin.x;
    screenCoord.y = bufferState.origin.y - coord.y / bufferState.pixelDimension.y;
    zoomToScreen(screenCoord, delta, direction);
}
void MandelbrotDrawer::computeMandelbrot(bool force)
{
    std::chrono::time_point<std::chrono::high_resolution_clock> t1, t2;

    if(!force && currentState == bufferState)
        return;
    currentState = bufferState;
    switch(currentState.computeMethod)
    {
        case 0:
            if(currentState.intrinsic)
            {
                t1 = high_resolution_clock::now();
                computeMandelbrotSingleThreadIntrinsic(currentState.origin,
                                                       currentState.dimension,
                                                       currentState.pixelDimension,
                                                       currentState.precision,
                                                       pixelIteration);
                t2 = high_resolution_clock::now();
            }
            else
            {
                t1 = high_resolution_clock::now();
                computeMandelbrotSingleThread(currentState.origin,
                                              currentState.dimension,
                                              currentState.pixelDimension,
                                              currentState.precision,
                                              pixelIteration);
                t2 = high_resolution_clock::now();
            }
            break;
        case 1:
            t1 = high_resolution_clock::now();
            computeMandelbrotOpenMP(currentState.origin,
                                    currentState.dimension,
                                    currentState.pixelDimension,
                                    currentState.precision,
                                    pixelIteration,
                                    currentState.nThread);
            t2 = high_resolution_clock::now();
            break;
        case 2:
            t1 = high_resolution_clock::now();
            computeMandelbrotThread(currentState.origin,
                                    currentState.dimension,
                                    currentState.pixelDimension,
                                    currentState.precision,
                                    pixelIteration,
                                    currentState.nThread);
            t2 = high_resolution_clock::now();
            break;
    }
    duration<double, std::milli> ms_double = t2 - t1;
    computeTime = ms_double.count();
    iterationToColorsGradient(pixelIteration,
                              currentState.dimension.x,
                              currentState.dimension.y,
                              currentState.precision,
                              gradient,
                              pixelColors);
    texture.update(pixelColors);
    sprite.setTexture(texture);
}
void MandelbrotDrawer::saveImage(char* savepath)
{
    sf::Image image;

    computeMandelbrot();
    image.create(currentState.dimension.x, currentState.dimension.y, pixelColors);
    image.saveToFile(savepath);
}
void MandelbrotDrawer::drawAxes(sf::RenderWindow& window)
{
    sf::Vertex axeX[2];
    sf::Vertex axeY[2];
    sf::Vertex midX[2];
    sf::Vertex midY[2];
    axeX[0].position = sf::Vector2f(0, currentState.origin.y);
    axeX[0].color = sf::Color::Red;
    axeX[1].position = sf::Vector2f(window.getSize().x, currentState.origin.y);
    axeX[1].color = sf::Color::Red;
    axeY[0].position = sf::Vector2f(currentState.origin.x, 0);
    axeY[0].color = sf::Color::Red;
    axeY[1].position = sf::Vector2f(currentState.origin.x, window.getSize().y);
    axeY[1].color = sf::Color::Red;

    midX[0].position = sf::Vector2f(0, currentState.dimension.y / 2);
    midX[0].color = sf::Color::Blue;
    midX[1].position = sf::Vector2f(window.getSize().x, currentState.dimension.y / 2);
    midX[1].color = sf::Color::Blue;
    midY[0].position = sf::Vector2f(currentState.dimension.x / 2, 0);
    midY[0].color = sf::Color::Blue;
    midY[1].position = sf::Vector2f(currentState.dimension.x / 2, window.getSize().y);
    midY[1].color = sf::Color::Blue;

    window.draw(axeX, 2, sf::Lines);
    window.draw(axeY, 2, sf::Lines);
    window.draw(midX, 2, sf::Lines);
    window.draw(midY, 2, sf::Lines);
}

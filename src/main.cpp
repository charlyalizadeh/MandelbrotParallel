#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Vector2.hpp>

#include <chrono>
#include <string>
#include <math.h>

#include "../lib/imgui/imgui.h"
#include "../lib/imgui/imgui-SFML.h"
#include "../include/MandelbrotSingleThread.hpp"
#include "../include/MandelbrotOpenMP.hpp"
#include "../include/MandelbrotThread.hpp"

using std::chrono::high_resolution_clock;
using std::chrono::duration;


struct InteractionState
{
    bool isDraging;
    sf::Vector2i mouseCoord;
};

struct MandelbrotState
{
    sf::Vector2i dimension;
    sf::Vector2i origin;
    sf::Vector2f pixelDimension;
    int precision;
    int computeMethod;
    int nThread;
    bool intrinsic;

    MandelbrotState(sf::Vector2i dimension,
                    sf::Vector2i origin,
                    sf::Vector2f pixelDimension,
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

};
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


void update(sf::RenderWindow& window, sf::Clock& deltaClock, MandelbrotState& mandelbrotState, InteractionState& interactionState)
{
    sf::Event event;

    while(window.pollEvent(event)) {
        ImGui::SFML::ProcessEvent(window, event);
        if(!ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow))
        {
            if(event.type == sf::Event::MouseWheelMoved)
            {
                auto mousePosition = sf::Mouse::getPosition(window);
                sf::Vector2f oldCoord = sf::Vector2f(mousePosition - mandelbrotState.origin);
                oldCoord.x *= mandelbrotState.pixelDimension.x;
                oldCoord.y *= mandelbrotState.pixelDimension.y;
                if(event.mouseWheel.delta > 0)
                {
                    mandelbrotState.pixelDimension.x -= 0.1 * event.mouseWheel.delta * mandelbrotState.pixelDimension.x;
                    mandelbrotState.pixelDimension.y -= 0.1 * event.mouseWheel.delta * mandelbrotState.pixelDimension.y;
                }
                else
                {
                    mandelbrotState.pixelDimension.x -= 0.1 * event.mouseWheel.delta * mandelbrotState.pixelDimension.x;
                    mandelbrotState.pixelDimension.y -= 0.1 * event.mouseWheel.delta * mandelbrotState.pixelDimension.y;
                }
                sf::Vector2f newCoord = sf::Vector2f(mousePosition - mandelbrotState.origin);
                newCoord.x *= mandelbrotState.pixelDimension.x;
                newCoord.y *= mandelbrotState.pixelDimension.y;
                sf::Vector2f offset = newCoord - oldCoord;
                offset.x /= mandelbrotState.pixelDimension.x;
                offset.y /= mandelbrotState.pixelDimension.y;
                mandelbrotState.origin += sf::Vector2i(offset);
            }
            if(!interactionState.isDraging && event.type == sf::Event::MouseButtonPressed)
            {
                interactionState.isDraging = true;
                interactionState.mouseCoord.x = event.mouseButton.x;
                interactionState.mouseCoord.y = event.mouseButton.y;
            }
        }
        if(interactionState.isDraging && event.type == sf::Event::MouseButtonReleased)
            interactionState.isDraging = false;
        if(event.type == sf::Event::Closed)
            window.close();
        if(event.type == sf::Event::Resized) {
            sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
            window.setView(sf::View(visibleArea));
        }
    }
    if(interactionState.isDraging)
    {
        auto mousePosition = sf::Mouse::getPosition(window);
        mandelbrotState.origin -= (interactionState.mouseCoord - mousePosition);
        interactionState.mouseCoord.x = mousePosition.x;
        interactionState.mouseCoord.y = mousePosition.y;
    }
    ImGui::SFML::Update(window, deltaClock.restart());
}
void drawImGui(sf::RenderWindow& window, MandelbrotState& mandelbrotState, const char** computeMethods, unsigned int nMethod, const std::string& mandelbrotTime)
{
    ImGui::Begin("Parameters");
    ImGui::ListBox("Computation method", &mandelbrotState.computeMethod, computeMethods, nMethod, 4);
    if(mandelbrotState.computeMethod == 0)
        ImGui::BeginDisabled();
    ImGui::InputInt("Num Thread", &mandelbrotState.nThread, 1, 64);
    if(mandelbrotState.computeMethod == 0)
        ImGui::EndDisabled();
    if(mandelbrotState.computeMethod != 0)
        ImGui::BeginDisabled();
    ImGui::Checkbox("Intrinsic: ", &mandelbrotState.intrinsic);
    if(mandelbrotState.computeMethod != 0)
        ImGui::EndDisabled();
    ImGui::InputInt("Precision", &mandelbrotState.precision, 1);

    auto mousePosition = sf::Mouse::getPosition(window);
    ImGui::LabelText("Time (ms)", mandelbrotTime.c_str());
    ImGui::LabelText("(Pixel)", "(X, Y): (%d, %d)", mousePosition.x, mousePosition.y);
    ImGui::LabelText("(Coord)", "(X, Y): (%f, %f)",
                (mousePosition.x - mandelbrotState.origin.x) * mandelbrotState.pixelDimension.x,
                (mousePosition.y - mandelbrotState.origin.y) * mandelbrotState.pixelDimension.y);
    ImGui::End();
}

int main()
{
    // Parameters
    MandelbrotState mState(sf::Vector2i(1920, 1080),
                           sf::Vector2i(1920 / 2 + 600, 1080 / 2),
                           sf::Vector2f(0.002, 0.002),
                           40,
                           0,
                           4,
                           true);
    const char* computeMethods[] = { "Single Thread", "OpenMP", "std::thread" };
    MandelbrotState mState2 = mState;

    // Zooming and draging the fractal
    InteractionState interactionState;
    interactionState.isDraging = false;


    // Timing of the mandelbrot computation
    std::string mandelbrotTime;

    // Pixel array
    sf::Uint8 *pixelColors = new sf::Uint8[mState.dimension.x * mState.dimension.y * 4];
    auto t1 = high_resolution_clock::now();
    auto t2 = high_resolution_clock::now();
    switch(mState.computeMethod)
    {
        case 0:
            if(mState.intrinsic)
            {
                t1 = high_resolution_clock::now();
                computeMandelbrotSingleThreadIntrinsic(mState.origin, mState.dimension, mState.pixelDimension, mState.precision, pixelColors);
                t2 = high_resolution_clock::now();
            }
            else
            {
                t1 = high_resolution_clock::now();
                computeMandelbrotSingleThread(mState.origin, mState.dimension, mState.pixelDimension, mState.precision, pixelColors);
                t2 = high_resolution_clock::now();
            }
            break;
        case 1:
            t1 = high_resolution_clock::now();
            computeMandelbrotOpenMP(mState.origin, mState.dimension, mState.pixelDimension, mState.precision, pixelColors, mState.nThread);
            t2 = high_resolution_clock::now();
            break;
        case 2:
            t1 = high_resolution_clock::now();
            computeMandelbrotThread(mState.origin, mState.dimension, mState.pixelDimension, mState.precision, pixelColors, mState.nThread);
            t2 = high_resolution_clock::now();
            break;
    }
    duration<double, std::milli> ms_double = t2 - t1;
    mandelbrotTime = std::to_string(ms_double.count());

    // Window
    sf::RenderWindow window(sf::VideoMode(mState.dimension.x, mState.dimension.y), "Mandelbrot");
    if(!ImGui::SFML::Init(window))
        return 0;

    // Texture and sprite to render
    sf::Texture texture;
    texture.create(mState.dimension.x, mState.dimension.y);
    texture.update(pixelColors);
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setPosition({0, 0});

    sf::Clock deltaClock;
    while(window.isOpen())
    {
        update(window, deltaClock, mState2, interactionState);

        // ImGui
        drawImGui(window, mState2, computeMethods, 3, mandelbrotTime);

        // If we have a state change we redraw the mandelbrot fractal
        if(mState != mState2)
        {
            mState = mState2;
            switch(mState.computeMethod)
            {
                case 0:
                    if(mState.intrinsic)
                    {
                        t1 = high_resolution_clock::now();
                        computeMandelbrotSingleThreadIntrinsic(mState.origin, mState.dimension, mState.pixelDimension, mState.precision, pixelColors);
                        t2 = high_resolution_clock::now();
                    }
                    else
                    {
                        t1 = high_resolution_clock::now();
                        computeMandelbrotSingleThread(mState.origin, mState.dimension, mState.pixelDimension, mState.precision, pixelColors);
                        t2 = high_resolution_clock::now();
                    }
                    break;
                case 1:
                    t1 = high_resolution_clock::now();
                    computeMandelbrotOpenMP(mState.origin, mState.dimension, mState.pixelDimension, mState.precision, pixelColors, mState.nThread);
                    t2 = high_resolution_clock::now();
                    break;
                case 2:
                    t1 = high_resolution_clock::now();
                    computeMandelbrotThread(mState.origin, mState.dimension, mState.pixelDimension, mState.precision, pixelColors, mState.nThread);
                    t2 = high_resolution_clock::now();
                    break;
            }

            duration<double, std::milli> ms_double = t2 - t1;
            mandelbrotTime = std::to_string(ms_double.count());

            texture.update(pixelColors);
            sprite.setTexture(texture);
        }
        window.clear(sf::Color::Black);
        window.draw(sprite);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    delete pixelColors;
    return 0;
}

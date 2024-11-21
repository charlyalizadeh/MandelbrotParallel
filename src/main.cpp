#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Vector2.hpp>

#include <chrono>
#include <string>

#include "../lib/imgui/imgui.h"
#include "../lib/imgui/imgui-SFML.h"
#include "../include/MandelbrotSingleThread.hpp"

using std::chrono::high_resolution_clock;
using std::chrono::duration;


struct MandelbrotState
{
    sf::Vector2i dimension;
    sf::Vector2i origin;
    sf::Vector2f pixelDimensions;
    int precision;
    int computeMethod;

    MandelbrotState(sf::Vector2i dimension,
                    sf::Vector2i origin,
                    sf::Vector2f pixelDimensions,
                    int precision,
                    int computeMethod) :
                        dimension(dimension),
                        origin(origin),
                        pixelDimensions(pixelDimensions),
                        precision(precision),
                        computeMethod(computeMethod) {}
};
bool operator !=(const MandelbrotState& m1, const MandelbrotState& m2)
{
    return (m1.dimension != m2.dimension ||
            m1.origin != m2.origin ||
            m1.pixelDimensions != m2.pixelDimensions ||
            m1.precision != m2.precision ||
            m1.computeMethod != m2.computeMethod);
}


void update(sf::RenderWindow& window, sf::Clock& deltaClock)
{
    sf::Event event;

    while(window.pollEvent(event)) {
        ImGui::SFML::ProcessEvent(window, event);
        if(event.type == sf::Event::Closed)
            window.close();
        if(event.type == sf::Event::Resized) {
            sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
            window.setView(sf::View(visibleArea));
        }
    }
    ImGui::SFML::Update(window, deltaClock.restart());
}
void drawImGui(MandelbrotState& mandelbrotState, const char** computeMethods, unsigned int nMethod, const std::string& mandelbrotTime)
{
    ImGui::Begin("Parameters");
    ImGui::ListBox("Computation method", &mandelbrotState.computeMethod, computeMethods, nMethod, 4);
    ImGui::LabelText("Time (ms)", mandelbrotTime.c_str());
    ImGui::SeparatorText("Origin");
    {
        ImGui::DragInt("Origin X", &mandelbrotState.origin.x, 1);
        ImGui::DragInt("Origin Y", &mandelbrotState.origin.y, 1);
    }
    ImGui::SeparatorText("Pixel dimension");
    {
        ImGui::DragFloat("PixelDim X", &mandelbrotState.pixelDimensions.x, 0.001);
        ImGui::DragFloat("PixelDim Y", &mandelbrotState.pixelDimensions.y, 0.001);
    }
    ImGui::SeparatorText("");
    {
        ImGui::DragInt("Precision", &mandelbrotState.precision, 1);
    }
    ImGui::End();
}

int main()
{
    // Parameters
    MandelbrotState mState(sf::Vector2i(1920, 1080),
                           sf::Vector2i(1920 / 2 + 200, 1080 / 2),
                           sf::Vector2f(0.002, 0.002),
                           150,
                           0);
    const char* computeMethods[] = { "Single Thread", "OpenMP" };
    MandelbrotState mState2 = mState;

    // Timing of the mandelbrot computation
    std::string mandelbrotTime;

    // Pixel array
    sf::Uint8 *pixelColors = new sf::Uint8[mState.dimension.x * mState.dimension.y * 4];
    auto t1 = high_resolution_clock::now();
    computeMandelbrotSingleThread(mState.origin,
                                  mState.dimension,
                                  mState.pixelDimensions,
                                  mState.precision,
                                  pixelColors);
    auto t2 = high_resolution_clock::now();
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
        // Close or resize the window
        update(window, deltaClock);

        // ImGui
        drawImGui(mState2, computeMethods, 2, mandelbrotTime);

        // If we have a state change we redraw the mandelbrot fractal
        if(mState != mState2)
        {
            mState = mState2;

            t1 = high_resolution_clock::now();
            computeMandelbrotSingleThread(mState.origin,
                                          mState.dimension,
                                          mState.pixelDimensions,
                                          mState.precision,
                                          pixelColors);
            t2 = high_resolution_clock::now();

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

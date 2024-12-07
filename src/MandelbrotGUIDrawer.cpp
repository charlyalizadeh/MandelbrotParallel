#include "../include/MandelbrotGUIDrawer.hpp"


const char* computeMethods[] = { "Single Thread", "OpenMP", "std::thread" };

void MandelbrotGUIDrawer::updateSFML(sf::RenderWindow& window)
{
    sf::Event event;

    while(window.pollEvent(event)) {
        ImGui::SFML::ProcessEvent(window, event);
        if(!ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow))
        {
            if(event.type == sf::Event::MouseWheelMoved)
            {
                auto mousePosition = sf::Mouse::getPosition(window);
                drawer.zoomToScreen(mousePosition, 0.1, event.mouseWheel.delta);
            }
            if(!isDraging && event.type == sf::Event::MouseButtonPressed)
            {
                isDraging = true;
                oldMouseCoord.x = event.mouseButton.x;
                oldMouseCoord.y = event.mouseButton.y;
            }
        }
        if(isDraging && event.type == sf::Event::MouseButtonReleased)
            isDraging = false;
        if(event.type == sf::Event::Closed)
            window.close();
        if(event.type == sf::Event::Resized) {
            sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
            window.setView(sf::View(visibleArea));
        }
    }
    if(isDraging)
    {
        auto mousePosition = sf::Mouse::getPosition(window);
        drawer.bufferState.origin -= (oldMouseCoord - mousePosition);
        oldMouseCoord.x = mousePosition.x;
        oldMouseCoord.y = mousePosition.y;
    }
}
void MandelbrotGUIDrawer::updateImGui(sf::RenderWindow& window)
{
    ImGui::SFML::Update(window, deltaClock.restart());
    ImGui::Begin("Parameters");
    ImGui::ListBox("Computation method", &drawer.bufferState.computeMethod, computeMethods, 3, 4);
    if(drawer.bufferState.computeMethod == 0)
        ImGui::BeginDisabled();
    ImGui::InputInt("Num Thread", &drawer.bufferState.nThread, 1, 64);
    if(drawer.bufferState.computeMethod == 0)
        ImGui::EndDisabled();
    if(drawer.bufferState.computeMethod != 0)
        ImGui::BeginDisabled();
    ImGui::Checkbox("Intrinsic: ", &drawer.bufferState.intrinsic);
    if(drawer.bufferState.computeMethod != 0)
        ImGui::EndDisabled();
    ImGui::InputInt("Precision", &drawer.bufferState.precision, 1);
    ImGui::InputText("Save Path", savepath, 128);
    if(ImGui::Button("Save"))
        drawer.saveImage(savepath);

    auto mousePosition = sf::Mouse::getPosition(window);
    auto mandelbrotTime = std::to_string(drawer.computeTime);
    ImGui::LabelText("Time (ms)", mandelbrotTime.c_str());
    ImGui::LabelText("Pixel", "(X, Y): (%d, %d)", mousePosition.x, mousePosition.y);
    ImGui::LabelText("Coord", "(X, Y): (%f, %f)",
                     (mousePosition.x - drawer.bufferState.origin.x) * drawer.bufferState.pixelDimension.x,
                     (drawer.bufferState.origin.y - mousePosition.y) * drawer.bufferState.pixelDimension.y);
    ImGui::LabelText("Zoom", "%.30f", drawer.bufferState.pixelDimension.x);
    ImGui::End();
}
void MandelbrotGUIDrawer::update(sf::RenderWindow& window)
{
    updateSFML(window);
    updateImGui(window);
}
void MandelbrotGUIDrawer::draw(sf::RenderWindow& window)
{
    window.clear(sf::Color::Black);
    window.draw(drawer.sprite);
}

MandelbrotGUIDrawer::MandelbrotGUIDrawer(const MandelbrotState& initState): drawer(initState), isDraging(false) {}
void MandelbrotGUIDrawer::start(sf::RenderWindow& window)
{
    if(!ImGui::SFML::Init(window))
        return;
    drawer.computeMandelbrot(true);
    while(window.isOpen())
    {
        update(window);
        drawer.computeMandelbrot();
        draw(window);
        ImGui::SFML::Render(window);
        window.display();
    }
    ImGui::SFML::Shutdown();
}

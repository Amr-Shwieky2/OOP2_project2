#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

class App {
public:
    App();
    ~App() = default;

    void run();  // Start the application
private:
    void initialize();     // Load resources, register screens
    void mainLoop();       // Main update/render loop

    std::unique_ptr<sf::RenderWindow> m_window;
};

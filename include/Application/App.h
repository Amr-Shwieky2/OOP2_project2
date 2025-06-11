#pragma once

#include <memory>
#include <SFML/Graphics.hpp>


class App {
public:
    App();
    ~App();

    // Starts the main loop. This will never return until the window is closed.
    void run();

private:

    void initialize();
    void initializeLanguageSystem();
    void loadLanguagePreference();
    // Load resources, register screens
    void mainLoop();       // Main update/render loop


    // Called each frame to update game logic (screen updates, physics, etc.)
    void update(float deltaTime);

    // Called each frame to draw everything to the window
    void render();

    sf::RenderWindow                   m_window;
    std::unique_ptr<AppContext>        m_context;
};

#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

// Forward‐declare classes that we’ll use
class AppContext;
class IScreen;

class App {
public:
    App();
    ~App();

    // Starts the main loop. This will never return until the window is closed.
    void run();

private:
    // Called each frame to process OS/window events (keyboard, mouse, close, etc.)
    void processEvents();

    // Called each frame to update game logic (screen updates, physics, etc.)
    void update(float deltaTime);

    // Called each frame to draw everything to the window
    void render();

    sf::RenderWindow                   m_window;
    std::unique_ptr<AppContext>        m_context;
};

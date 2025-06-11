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
<<<<<<< HEAD
    void initialize();
    void initializeLanguageSystem();
    void loadLanguagePreference();
    // Load resources, register screens
    void mainLoop();       // Main update/render loop
=======
    // Called each frame to process OS/window events (keyboard, mouse, close, etc.)
    void processEvents();
>>>>>>> 70f8dadc4c8b82d7171402a7e9888286f78eafbe

    // Called each frame to update game logic (screen updates, physics, etc.)
    void update(float deltaTime);

    // Called each frame to draw everything to the window
    void render();

    sf::RenderWindow                   m_window;
    std::unique_ptr<AppContext>        m_context;
};

#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

class App {
public:
    App();
    ~App() = default;

    void run();  // Start the application
private:
    void initialize();
    void initializeAudioSystem();
    void initializeLanguageSystem();
    void loadLanguagePreference();
    // Load resources, register screens
    void mainLoop();       // Main update/render loop

    void cleanup();

    std::unique_ptr<sf::RenderWindow> m_window;
};

// File: src/Application/App.cpp
#include "App.h"
#include "AppContext.h"
#include "ScreenTypes.h"
#include "Logger.h"

// Include screen files
#include "../Screens/LoadingScreen.h"
#include "../Screens/MenuScreen.h"
#include "../Screens/GameplayScreen.h"
#include "../Screens/SettingsScreen.h"

App::App() {
    // Create window
    m_window = std::make_unique<sf::RenderWindow>(
        sf::VideoMode(1400, 800), "Desert Ball"
    );
    m_window->setFramerateLimit(60);

    // Set icon if you want
    // sf::Image icon;
    // if (icon.loadFromFile("assets/icon.png")) {
    //     m_window->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    // }
}

void App::run() {
    Logger::log("Starting game...");
    initialize();
    mainLoop();
    Logger::log("Game ended.");
}

void App::initialize() {
    auto& screenManager = AppContext::instance().screenManager();

    // Register all screens
    screenManager.registerScreen(ScreenType::LOADING, []() {
        return std::make_unique<LoadingScreen>();
        });

    screenManager.registerScreen(ScreenType::MENU, []() {
        return std::make_unique<MenuScreen>();
        });

    screenManager.registerScreen(ScreenType::PLAY, []() {
        return std::make_unique<GameplayScreen>();
        });

    screenManager.registerScreen(ScreenType::SETTINGS, []() {
        return std::make_unique<SettingsScreen>();
        });

    // Start with loading screen
    screenManager.changeScreen(ScreenType::LOADING);
    Logger::log("All screens initialized");
}

void App::mainLoop() {
    sf::Clock clock;
    auto& screenManager = AppContext::instance().screenManager();

    while (m_window->isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        // Handle events
        screenManager.handleEvents(*m_window);

        // Update game
        screenManager.update(deltaTime);

        // Draw everything
        m_window->clear(sf::Color::Black);
        screenManager.render(*m_window);
        m_window->display();
    }
}
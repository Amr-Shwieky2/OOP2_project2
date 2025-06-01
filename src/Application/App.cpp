#include "App.h"

#include "AppContext.h"
#include "ScreenTypes.h"
#include "MenuScreen.h"
#include "HelpScreen.h"
#include "SettingsScreen.h"
#include "PauseScreen.h"
#include "Logger.h"

App::App() {
    m_window = std::make_unique<sf::RenderWindow>(
        sf::VideoMode(800, 600), "Game Menu System"
    );
    m_window->setFramerateLimit(60);
}

void App::run() {
    Logger::log("App starting...");
    initialize();
    mainLoop();
    Logger::log("App exited.");
}

void App::initialize() {
    auto& screenManager = AppContext::instance().screenManager();

    screenManager.registerScreen(ScreenType::Menu, []() {
        return std::make_unique<MenuScreen>();
        });
    screenManager.registerScreen(ScreenType::Help, []() {
        return std::make_unique<HelpScreen>();
        });
    screenManager.registerScreen(ScreenType::Settings, []() {
        return std::make_unique<SettingsScreen>();
        });
    screenManager.registerScreen(ScreenType::Pause, []() {
        return std::make_unique<PauseScreen>();
        });

    screenManager.changeScreen(ScreenType::Menu);
    Logger::log("Screens initialized");
}

void App::mainLoop() {
    sf::Clock clock;

    auto& screenManager = AppContext::instance().screenManager();

    while (m_window->isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        screenManager.handleEvents(*m_window);
        screenManager.update(deltaTime);

        m_window->clear();
        screenManager.render(*m_window);
        m_window->display();
    }
}

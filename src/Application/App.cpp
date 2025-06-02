#include "App.h"
#include "AppContext.h"
#include "IScreen.h"
#include "ScreenManager.h"

// If you have a default “first screen” class (e.g. MainMenuScreen), include it here:
#include "MainMenuScreen.h"

App::App()
    : m_window(sf::VideoMode(1280, 720), "Desert Ball")  // choose your desired window size and title
{
    // Create the AppContext. It will construct all major services (Audio, Resources, Physics, etc.).
    m_context = std::make_unique<AppContext>(m_window);

    // Immediately push the first screen onto the ScreenManager (e.g. main menu)
    // We assume AppContext gives access to ScreenManager via getScreenManager().
    m_context->getScreenManager().pushScreen(std::make_unique<MainMenuScreen>(*m_context));
}

App::~App() {
    // All resources (AppContext, screens, services) are cleaned up automatically.
}

void App::run() {
    sf::Clock clock;

    while (m_window.isOpen()) {
        // 1) Handle all pending window/events (keyboard/mouse/close)
        processEvents();

        // 2) Update logic with the elapsed time since last frame
        float deltaTime = clock.restart().asSeconds();
        update(deltaTime);

        // 3) Render everything to the window
        render();
    }
}

void App::processEvents() {
    sf::Event event;
    while (m_window.pollEvent(event)) {
        // If the user pressed the close button, close the window
        if (event.type == sf::Event::Closed) {
            m_window.close();
            return;
        }
        // Forward the event to the active screen so it can handle input
        m_context->getScreenManager().handleInput(m_window);
    }
}

void App::update(float deltaTime) {
    // Let the active screen update itself
    m_context->getScreenManager().update(deltaTime);
}

void App::render() {
    // Clear to a background color
    m_window.clear(sf::Color(30, 30, 30));

    // Let the active screen draw itself
    m_context->getScreenManager().render(m_window);

    // Finally display on screen
    m_window.display();
}

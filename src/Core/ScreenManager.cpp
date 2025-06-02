#include "ScreenManager.h"
#include <iostream>

ScreenManager::ScreenManager() = default;

ScreenManager::~ScreenManager() {
    // When ScreenManager is destroyed, its stack of unique_ptr<IScreen> will automatically clean up.
    // However, ensure that any onExit() is called on each before destruction (optional).
    while (!m_screens.empty()) {
        m_screens.top()->onExit();
        m_screens.pop();
    }
}

void ScreenManager::pushScreen(std::unique_ptr<IScreen> screen) {
    if (!m_screens.empty()) {
        // Tell the old top screen that it’s being paused/exited
        m_screens.top()->onExit();
    }
    // Push and then immediately notify the new top with onEnter()
    m_screens.push(std::move(screen));
    m_screens.top()->onEnter();
}

void ScreenManager::popScreen() {
    if (m_screens.empty()) {
        return;
    }
    // Tell the top screen it’s exiting, then pop it
    m_screens.top()->onExit();
    m_screens.pop();

    // If there is now another screen underneath, notify it that it’s being re‐entered
    if (!m_screens.empty()) {
        m_screens.top()->onEnter();
    }
}

void ScreenManager::changeScreen(std::unique_ptr<IScreen> screen) {
    // If there is an existing top screen, tell it to exit and remove it
    if (!m_screens.empty()) {
        m_screens.top()->onExit();
        m_screens.pop();
    }
    // Push the new screen and notify it of entry
    m_screens.push(std::move(screen));
    m_screens.top()->onEnter();
}

IScreen* ScreenManager::getActiveScreen() {
    if (m_screens.empty()) {
        return nullptr;
    }
    return m_screens.top().get();
}

void ScreenManager::handleInput(sf::RenderWindow& window) {
    if (auto active = getActiveScreen()) {
        active->handleInput(window);
    }
}

void ScreenManager::update(float deltaTime) {
    if (auto active = getActiveScreen()) {
        active->update(deltaTime);
    }
}

void ScreenManager::render(sf::RenderWindow& window) {
    if (auto active = getActiveScreen()) {
        active->render(window);
    }
}

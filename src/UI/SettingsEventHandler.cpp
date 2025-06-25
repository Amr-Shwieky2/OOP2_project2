#include "SettingsEventHandler.h"
#include "VolumeControlPanel.h"
#include <iostream>

SettingsEventHandler::SettingsEventHandler() : m_shouldExitToMenu(false) {}

SettingsEventHandler::~SettingsEventHandler() {
    cleanup();
}

void SettingsEventHandler::cleanup() {
    try {
        m_shouldExitToMenu = true;

        if (m_volumePanel) {
            m_volumePanel.reset();
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error during cleanup: " << e.what() << std::endl;
    }
}

void SettingsEventHandler::setVolumePanel(std::shared_ptr<VolumeControlPanel> panel) {
    if (panel) {
        m_volumePanel = panel;
    }
}

void SettingsEventHandler::resetExitFlag() {
    m_shouldExitToMenu = false;
    std::cout << "Exit flag reset" << std::endl;
}

bool SettingsEventHandler::shouldExitToMenu() const {
    return m_shouldExitToMenu;
}

void SettingsEventHandler::handleEvents(sf::RenderWindow& window) {
    if (m_shouldExitToMenu) {
        return;
    }

    sf::Event event;
    while (window.pollEvent(event)) {
        try {
            handleWindowEvents(window, event);
            handleKeyboardEvents(event);
            handleMouseEvents(event);
        }
        catch (const std::exception& e) {
            std::cout << "Error handling event: " << e.what() << std::endl;
            m_shouldExitToMenu = true;
        }
        catch (...) {
            std::cout << "Unknown error handling event" << std::endl;
            m_shouldExitToMenu = true;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        if (!m_shouldExitToMenu) {
            std::cout << "Escape detected via polling - requesting exit to menu" << std::endl;
            m_shouldExitToMenu = true;
        }
    }
}

void SettingsEventHandler::handleWindowEvents(sf::RenderWindow& window, const sf::Event& event) {
    if (event.type == sf::Event::Closed) {
        std::cout << "Window close event received" << std::endl;
        window.close();
    }
}

void SettingsEventHandler::handleKeyboardEvents(const sf::Event& event) {
    if (!isValidKeyboardEvent(event)) return;

    if (event.type == sf::Event::KeyPressed) {
        std::cout << "Key pressed detected: " << event.key.code << std::endl;
        handleKeyPressEvents(event);
    }
}

void SettingsEventHandler::handleKeyPressEvents(const sf::Event& event) {
    std::cout << "Processing key code: " << event.key.code << " (Escape=" << sf::Keyboard::Escape << ")" << std::endl;

    switch (event.key.code) {
    case sf::Keyboard::Escape:
        std::cout << "Escape case matched!" << std::endl;
        processEscapeKey();
        break;

    case sf::Keyboard::Enter:
        processEnterKey();
        break;

    default:
        std::cout << "Unhandled key: " << event.key.code << std::endl;
        break;
    }
}

void SettingsEventHandler::handleMouseEvents(const sf::Event& event) {
    if (!isValidMouseEvent(event)) return;

    switch (event.type) {
    case sf::Event::MouseMoved:
        handleMouseMoveEvents(event);
        break;

    case sf::Event::MouseButtonPressed:
        handleMousePressEvents(event);
        break;

    case sf::Event::MouseButtonReleased:
        handleMouseReleaseEvents(event);
        break;

    default:
        break;
    }
}

void SettingsEventHandler::handleMouseMoveEvents(const sf::Event& event) {
    delegateMouseEventToComponents(event);
}

void SettingsEventHandler::handleMousePressEvents(const sf::Event& event) {
    if (event.mouseButton.button == sf::Mouse::Left) {
        bool handled = delegateMouseEventToComponents(event);
        if (handled) {
            std::cout << "Mouse press handled by component" << std::endl;
        }
    }
}

void SettingsEventHandler::handleMouseReleaseEvents(const sf::Event& event) {
    if (event.mouseButton.button == sf::Mouse::Left) {
        delegateMouseEventToComponents(event);
    }
}

bool SettingsEventHandler::delegateMouseEventToComponents(const sf::Event& event) {
    if (m_shouldExitToMenu) {
        return false;
    }

    bool handled = false;

    try {
        if (m_volumePanel && m_volumePanel.get() != nullptr) {
            handled |= m_volumePanel->handleMouseEvent(event);
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error delegating mouse event: " << e.what() << std::endl;
        cleanup();
        m_shouldExitToMenu = true;
    }

    return handled;
}

void SettingsEventHandler::processEscapeKey() {
    if (!m_shouldExitToMenu) {
        std::cout << "Escape key pressed - requesting exit to menu" << std::endl;
        std::cout << "Setting exit flag to true" << std::endl;
        m_shouldExitToMenu = true;
    }
    else {
        std::cout << "Escape already processed, ignoring" << std::endl;
    }
}

void SettingsEventHandler::processEnterKey() {
    std::cout << "Enter key pressed in settings" << std::endl;

    try {
        if (m_volumePanel && m_volumePanel.get() != nullptr && m_volumePanel->hasChanged()) {
            m_volumePanel->saveSettings();
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error saving settings on Enter: " << e.what() << std::endl;
    }
}

bool SettingsEventHandler::isValidMouseEvent(const sf::Event& event) const {
    switch (event.type) {
    case sf::Event::MouseMoved:
    case sf::Event::MouseButtonPressed:
    case sf::Event::MouseButtonReleased:
        return true;
    default:
        return false;
    }
}

bool SettingsEventHandler::isValidKeyboardEvent(const sf::Event& event) const {
    switch (event.type) {
    case sf::Event::KeyPressed:
    case sf::Event::KeyReleased:
        return true;
    default:
        return false;
    }
}
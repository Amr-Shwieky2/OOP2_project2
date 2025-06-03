#include "../../include/Screens/HelpScreen.h"
#include "AppContext.h"
#include "ScreenTypes.h"

HelpScreen::HelpScreen() {
    // Setup font
    try {
        m_font.loadFromFile("assets/fonts/arial.ttf");
    }
    catch (...) {
        // Default font
    }

    // Title
    m_titleText.setFont(m_font);
    m_titleText.setString("Help");
    m_titleText.setCharacterSize(48);
    m_titleText.setFillColor(sf::Color::Yellow);
    m_titleText.setPosition(350, 100);

    // Help content
    m_helpText.setFont(m_font);
    m_helpText.setString("How to play:\n\n- Use Arrow Keys to navigate menus\n- Press Enter to select\n- Use A/D keys to move in game\n- Press ESC to go back");
    m_helpText.setCharacterSize(24);
    m_helpText.setFillColor(sf::Color::White);
    m_helpText.setPosition(200, 200);

    // Back instruction
    m_backText.setFont(m_font);
    m_backText.setString("Press ESC to go back");
    m_backText.setCharacterSize(20);
    m_backText.setFillColor(sf::Color(128, 128, 128)); // Use RGB values for gray color
    m_backText.setPosition(300, 500);
}

void HelpScreen::handleEvents(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                AppContext::instance().screenManager().changeScreen(ScreenType::MENU);
            }
        }
    }
}

void HelpScreen::update(float deltaTime) {
    // Help screen logic here
}

void HelpScreen::render(sf::RenderWindow& window) {
    window.draw(m_titleText);
    window.draw(m_helpText);
    window.draw(m_backText);
}
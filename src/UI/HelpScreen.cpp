#include "HelpScreen.h"
#include "AppContext.h"
#include "Logger.h"
#include "ScreenTypes.h"

HelpScreen::HelpScreen() {
    try {
        m_font = AppContext::instance().resources().getFont("arial.ttf");

        m_title.setFont(m_font);
        m_title.setString("Help");
        m_title.setCharacterSize(40);
        m_title.setFillColor(sf::Color::White);
        m_title.setPosition(100.f, 50.f);

        m_instructions.setFont(m_font);
        m_instructions.setString(
            "Use arrow keys to move.\n"
            "Press ESC to pause.\n"
            "Avoid obstacles and survive as long as you can.");
        m_instructions.setCharacterSize(22);
        m_instructions.setFillColor(sf::Color::White);
        m_instructions.setPosition(100.f, 120.f);

        m_backButton.setFont(m_font);
        m_backButton.setString("Back");
        m_backButton.setCharacterSize(28);
        m_backButton.setFillColor(sf::Color::White);
        m_backButton.setPosition(100.f, 300.f);
        m_backBounds = m_backButton.getGlobalBounds();

        Logger::log("HelpScreen initialized");
    }
    catch (const std::exception& e) {
        Logger::log(std::string("HelpScreen init failed: ") + e.what(), LogLevel::Error);
    }
}

void HelpScreen::handleEvents(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::MouseMoved)
            updateHover(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            handleClick(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
    }
}

void HelpScreen::update(float) {
    // Optional animations or timing logic
}

void HelpScreen::render(sf::RenderWindow& window) {
    window.draw(m_title);
    window.draw(m_instructions);
    window.draw(m_backButton);
}

void HelpScreen::updateHover(const sf::Vector2i& mousePos) {
    if (m_backBounds.contains(static_cast<sf::Vector2f>(mousePos)))
        m_backButton.setFillColor(sf::Color::Yellow);
    else
        m_backButton.setFillColor(sf::Color::White);
}

void HelpScreen::handleClick(const sf::Vector2i& mousePos) {
    if (m_backBounds.contains(static_cast<sf::Vector2f>(mousePos))) {
        Logger::log("Back to menu clicked.");
        AppContext::instance().audio().playSound("click");
        AppContext::instance().screenManager().changeScreen(ScreenType::Menu);
    }
}

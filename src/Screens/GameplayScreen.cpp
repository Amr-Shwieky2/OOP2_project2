// File: src/Screens/GameplayScreen.cpp
#include "AppContext.h"
#include "ScreenTypes.h"
#include "../../include/Screens/GameplayScreen.h"

GameplayScreen::GameplayScreen() {
    m_gameText.setString("Game Screen - Press ESC to go back");
    m_gameText.setCharacterSize(24);
    m_gameText.setFillColor(sf::Color::White);
    m_gameText.setPosition(200, 200);

    m_player.setRadius(25);
    m_player.setFillColor(sf::Color::Blue);
    m_player.setPosition(400, 300);
}

void GameplayScreen::handleEvents(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            AppContext::instance().screenManager().changeScreen(ScreenType::MENU);
        }
    }
}

void GameplayScreen::update(float deltaTime) {
    // Simple player movement
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        m_playerVelocity.x = -200;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        m_playerVelocity.x = 200;
    }
    else {
        m_playerVelocity.x = 0;
    }

    sf::Vector2f pos = m_player.getPosition();
    pos += m_playerVelocity * deltaTime;
    m_player.setPosition(pos);
}

void GameplayScreen::render(sf::RenderWindow& window) {
    window.draw(m_gameText);
    window.draw(m_player);
}
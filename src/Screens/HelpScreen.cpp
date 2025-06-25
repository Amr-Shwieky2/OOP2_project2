﻿#include "../../include/Screens/HelpScreen.h"
#include "AppContext.h"
#include "ScreenTypes.h"
#include "AudioManager.h"

HelpScreen::HelpScreen() {
    try {
        m_backgroundTexture = AppContext::instance().getTexture("HelpScreen.png");
        m_backgroundSprite.setTexture(m_backgroundTexture);

        sf::Vector2u textureSize = m_backgroundTexture.getSize();
        if (textureSize.x > 0 && textureSize.y > 0) {
            sf::Vector2f targetSize(1400.0f, 800.0f);
            float scaleX = targetSize.x / textureSize.x;
            float scaleY = targetSize.y / textureSize.y;
            m_backgroundSprite.setScale(scaleX, scaleY);
        }
    }
    catch (...) {
        std::cout << "Error: Could not load HelpScreen.png!" << std::endl;
    }
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
}

void HelpScreen::render(sf::RenderWindow& window) {
    window.draw(m_backgroundSprite);
}
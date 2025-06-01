#pragma once

#include "IScreen.h"
#include <SFML/Graphics.hpp>

class HelpScreen : public IScreen {
public:
    HelpScreen();

    void handleEvents(sf::RenderWindow& window) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    sf::Font m_font;
    sf::Text m_title;
    sf::Text m_instructions;
    sf::Text m_backButton;
    sf::FloatRect m_backBounds;

    void handleClick(const sf::Vector2i& mousePos);
    void updateHover(const sf::Vector2i& mousePos);
};

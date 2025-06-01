#pragma once

#include "IScreen.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class PauseScreen : public IScreen {
public:
    PauseScreen();

    void handleEvents(sf::RenderWindow& window) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    sf::Font m_font;
    sf::Text m_title;
    std::vector<sf::Text> m_options;
    std::vector<sf::FloatRect> m_optionBounds;
    std::vector<std::string> m_labels = { "Resume", "Settings", "Exit to Menu" };

    void updateHover(const sf::Vector2i& mousePos);
    void handleClick(const sf::Vector2i& mousePos);
};

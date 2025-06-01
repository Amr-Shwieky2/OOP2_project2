#pragma once

#include "IScreen.h"
#include "ScreenManager.h"
#include "AppContext.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class MenuScreen : public IScreen {
public:
    MenuScreen();

    void handleEvents(sf::RenderWindow& window) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    void updateHoverEffect(const sf::Vector2i& mousePos);
    void handleClick(const sf::Vector2i& mousePos);

    sf::Font m_font;
    sf::Sprite m_background;
    std::vector<sf::Text> m_menuItems;
    std::vector<sf::FloatRect> m_itemBounds;

    std::vector<std::string> m_labels = { "Play", "Help", "Settings", "Exit" };
};

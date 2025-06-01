#pragma once

#include "IScreen.h"
#include <SFML/Graphics.hpp>

class SettingsScreen : public IScreen {
public:
    SettingsScreen();

    void handleEvents(sf::RenderWindow& window) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    sf::Font m_font;
    sf::Text m_title;
    sf::Text m_musicLabel;
    sf::Text m_effectsLabel;
    sf::Text m_backButton;

    sf::RectangleShape m_musicSlider;
    sf::RectangleShape m_effectsSlider;

    float m_musicVolume = 100.f;
    float m_effectsVolume = 100.f;

    sf::FloatRect m_backBounds;

    void handleClick(const sf::Vector2i& pos);
    void updateHover(const sf::Vector2i& pos);
    void handleSliderDrag(const sf::Vector2i& pos);
    bool m_draggingMusic = false;
    bool m_draggingEffects = false;
};

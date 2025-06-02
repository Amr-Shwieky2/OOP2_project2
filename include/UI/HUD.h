#pragma once

#include "Widget.h"
#include <SFML/Graphics.hpp>
#include <string>

///
/// HUD
/// ---
/// Displays game information (score, lives, etc.) in a fixed screen position.
/// You can expand this to show multiple text fields or icons.
///
class HUD : public Widget {
public:
    HUD();
    ~HUD() override;

    // Set the score to display
    void setScore(int score);

    // Set the number of lives to display
    void setLives(int lives);

    // Must set a font before calling setScore or setLives
    void setFont(const sf::Font& font);
    void setCharacterSize(unsigned int size);

    // handleEvent does nothing for HUD
    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;

    // update the internal text positions (if needed)
    void update(float deltaTime) override;

    // draw the HUD elements
    void render(sf::RenderTarget& target) override;

private:
    sf::Text m_scoreText;
    sf::Text m_livesText;

    int     m_score = 0;
    int     m_lives = 0;
};

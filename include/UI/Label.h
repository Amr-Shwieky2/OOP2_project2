#pragma once

#include "Widget.h"
#include <SFML/Graphics.hpp>
#include <string>

///
/// Label
/// -----
/// Simple text label. Non‐interactive (handleEvent does nothing).
/// You can change its string, font, size, and color.
///
class Label : public Widget {
public:
    Label();
    ~Label() override;

    void setText(const std::string& str);
    void setFont(const sf::Font& font);
    void setCharacterSize(unsigned int size);
    void setColor(const sf::Color& color);

    // Label does not react to events
    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;

    // Update position if needed
    void update(float deltaTime) override;

    // Draw the text
    void render(sf::RenderTarget& target) override;

private:
    sf::Text m_text;
};

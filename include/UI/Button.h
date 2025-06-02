#pragma once

#include "Widget.h"
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

///
/// Button
/// ------
/// A rectangular clickable UI element. Displays text centered inside.
/// When clicked (left mouse down inside its bounds), calls the stored onClick callback.
///
class Button : public Widget {
public:
    Button();
    ~Button() override;

    // Set the text string to display
    void setText(const std::string& str);

    // Set the font to use (caller is responsible for loading the font via ResourceLoader)
    void setFont(const sf::Font& font);

    // Set the character size in pixels
    void setCharacterSize(unsigned int size);

    // Provide a callback to be invoked when the user clicks this button
    void setOnClick(const std::function<void()>& callback);

    // Handle mouse events
    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;

    // Update (e.g. highlight on hover, animate, etc.)
    void update(float deltaTime) override;

    // Draw the button (rectangle + text)
    void render(sf::RenderTarget& target) override;

private:
    sf::RectangleShape   m_shape;
    sf::Text             m_text;
    std::function<void()> m_onClick;
};

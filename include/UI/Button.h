<<<<<<< HEAD
// ============= File: include/UI/Button.h =============
#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <functional>

class Button {
public:
    Button();
    Button(const sf::Vector2f& position, const sf::Vector2f& size, const std::string& text);

    // Basic properties
    void setPosition(const sf::Vector2f& position);
    void setSize(const sf::Vector2f& size);
    void setText(const std::string& text);
    void setFont(const sf::Font& font);

    // Simple image support
    void setButtonImage(const sf::Texture* texture);

    // Simple colors
    void setBackgroundColor(const sf::Color& color);
    void setTextColor(const sf::Color& color);

    // Events
    bool isMouseOver(const sf::Vector2f& mousePos) const;
    bool handleClick(const sf::Vector2f& mousePos);
    void handleMouseMove(const sf::Vector2f& mousePos); // New for hover effects

    // Update and rendering
    void update(float deltaTime); // New for animations
    void render(sf::RenderWindow& window);

    // Callback
    void setCallback(std::function<void()> callback);

private:
    sf::RectangleShape m_background;
    sf::Sprite m_sprite;
    sf::Text m_text;

    const sf::Texture* m_texture = nullptr;
    sf::Color m_backgroundColor = sf::Color(100, 100, 100, 255);
    sf::Color m_textColor = sf::Color::White;

    std::function<void()> m_callback;

    // Hover effects
    bool m_isHovered = false;
    float m_hoverScale = 1.0f;
    float m_targetScale = 1.0f;
    sf::Vector2f m_originalPosition;
    sf::Vector2f m_originalSize;

    void updateTextPosition();
    void updateHoverEffects(float deltaTime);
};
=======
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
>>>>>>> 70f8dadc4c8b82d7171402a7e9888286f78eafbe

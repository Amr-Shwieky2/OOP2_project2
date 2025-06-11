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
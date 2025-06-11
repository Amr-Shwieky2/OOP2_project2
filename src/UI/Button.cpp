<<<<<<< HEAD
// ============= File: src/UI/Button.cpp =============
#include "Button.h"

Button::Button() {
    // Setup background
    m_background.setFillColor(m_backgroundColor);
    m_background.setOutlineColor(sf::Color::White);
    m_background.setOutlineThickness(2);

    // Setup text
    m_text.setFillColor(m_textColor);
    m_text.setCharacterSize(24);
    m_text.setStyle(sf::Text::Bold);

    // Setup sprite
    m_sprite.setColor(sf::Color::White);

    // Initialize hover effects
    m_hoverScale = 1.0f;
    m_targetScale = 1.0f;
    m_isHovered = false;
}

Button::Button(const sf::Vector2f& position, const sf::Vector2f& size, const std::string& text)
    : Button() {
    setPosition(position);
    setSize(size);
    setText(text);
}

void Button::setPosition(const sf::Vector2f& position) {
    m_originalPosition = position;
    m_background.setPosition(position);
    m_sprite.setPosition(position);
    updateTextPosition();
}

void Button::setSize(const sf::Vector2f& size) {
    m_originalSize = size;
    m_background.setSize(size);

    // Scale sprite if we have texture
    if (m_texture) {
        sf::Vector2u textureSize = m_texture->getSize();
        float scaleX = size.x / textureSize.x;
        float scaleY = size.y / textureSize.y;
        m_sprite.setScale(scaleX, scaleY);
    }

    updateTextPosition();
}

void Button::setText(const std::string& text) {
    m_text.setString(text);
    updateTextPosition();
=======
﻿#include "Button.h"

Button::Button() {
    // Default shape colors
    m_shape.setFillColor(sf::Color(50, 50, 50));       // dark gray
    m_shape.setOutlineColor(sf::Color::White);         // white border
    m_shape.setOutlineThickness(2.f);
}

Button::~Button() {
}

void Button::setText(const std::string& str) {
    m_text.setString(str);
    // Recenter text:
    auto bounds = m_text.getLocalBounds();
    m_text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
    m_text.setPosition(m_position + (m_size / 2.0f));
>>>>>>> 70f8dadc4c8b82d7171402a7e9888286f78eafbe
}

void Button::setFont(const sf::Font& font) {
    m_text.setFont(font);
<<<<<<< HEAD
    updateTextPosition();
}

void Button::setButtonImage(const sf::Texture* texture) {
    m_texture = texture;
    if (m_texture) {
        m_sprite.setTexture(*m_texture);
        setSize(m_background.getSize()); // Update scaling
    }
}

void Button::setBackgroundColor(const sf::Color& color) {
    m_backgroundColor = color;
    m_background.setFillColor(color);
}

void Button::setTextColor(const sf::Color& color) {
    m_textColor = color;
    m_text.setFillColor(color);
}

bool Button::isMouseOver(const sf::Vector2f& mousePos) const {
    return m_background.getGlobalBounds().contains(mousePos);
}

void Button::handleMouseMove(const sf::Vector2f& mousePos) {
    bool wasHovered = m_isHovered;
    m_isHovered = isMouseOver(mousePos);

    if (m_isHovered && !wasHovered) {
        // Mouse entered - start hover effect
        m_targetScale = 1.1f; // Scale up by 10%
    }
    else if (!m_isHovered && wasHovered) {
        // Mouse left - end hover effect
        m_targetScale = 1.0f; // Back to normal
    }
}

bool Button::handleClick(const sf::Vector2f& mousePos) {
    if (isMouseOver(mousePos)) {
        if (m_callback) {
            m_callback();
        }
        return true;
    }
    return false;
}

void Button::update(float deltaTime) {
    updateHoverEffects(deltaTime);
}

void Button::render(sf::RenderWindow& window) {
    // Draw glow effect when hovered
    if (m_isHovered) {
        sf::RectangleShape glow = m_background;
        glow.setSize(sf::Vector2f(
            m_originalSize.x + 20,
            m_originalSize.y + 20
        ));
        glow.setPosition(sf::Vector2f(
            m_originalPosition.x - 10,
            m_originalPosition.y - 10
        ));
        glow.setFillColor(sf::Color(255, 255, 255, 30)); // White glow
        glow.setOutlineColor(sf::Color(255, 255, 0, 100)); // Yellow outline
        glow.setOutlineThickness(3);
        window.draw(glow);
    }

    // If we have texture, draw it, otherwise draw background
    if (m_texture) {
        window.draw(m_sprite);
    }
    else {
        window.draw(m_background);
    }

    // Draw text if it's visible
    if (m_text.getFillColor().a > 0) {
        window.draw(m_text);
    }
}

void Button::setCallback(std::function<void()> callback) {
    m_callback = callback;
}

void Button::updateTextPosition() {
    sf::FloatRect bounds = m_background.getGlobalBounds();
    sf::FloatRect textBounds = m_text.getLocalBounds();

    m_text.setPosition(
        bounds.left + (bounds.width - textBounds.width) / 2,
        bounds.top + (bounds.height - textBounds.height) / 2 - textBounds.top
    );
}

void Button::updateHoverEffects(float deltaTime) {
    // Smooth scale animation
    float scaleSpeed = 5.0f; // Speed of animation

    if (m_hoverScale < m_targetScale) {
        m_hoverScale += scaleSpeed * deltaTime;
        if (m_hoverScale > m_targetScale) m_hoverScale = m_targetScale;
    }
    else if (m_hoverScale > m_targetScale) {
        m_hoverScale -= scaleSpeed * deltaTime;
        if (m_hoverScale < m_targetScale) m_hoverScale = m_targetScale;
    }

    // Apply scaling to elements
    sf::Vector2f scaledSize = m_originalSize * m_hoverScale;
    sf::Vector2f offset = (scaledSize - m_originalSize) * 0.5f;
    sf::Vector2f scaledPosition = m_originalPosition - offset;

    m_background.setSize(scaledSize);
    m_background.setPosition(scaledPosition);

    if (m_texture) {
        sf::Vector2u textureSize = m_texture->getSize();
        float scaleX = (scaledSize.x / textureSize.x);
        float scaleY = (scaledSize.y / textureSize.y);
        m_sprite.setScale(scaleX, scaleY);
        m_sprite.setPosition(scaledPosition);
    }

    updateTextPosition();
}
=======
}

void Button::setCharacterSize(unsigned int size) {
    m_text.setCharacterSize(size);
    // Re‐center:
    auto bounds = m_text.getLocalBounds();
    m_text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
    m_text.setPosition(m_position + (m_size / 2.0f));
}

void Button::setOnClick(const std::function<void()>& callback) {
    m_onClick = callback;
}

void Button::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (!m_visible || !m_enabled) return;

    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f mousePosF(static_cast<float>(event.mouseButton.x),
            static_cast<float>(event.mouseButton.y));
        if (m_shape.getGlobalBounds().contains(mousePosF)) {
            if (m_onClick) {
                m_onClick();
            }
        }
    }
}

void Button::update(float deltaTime) {
    if (!m_visible) return;

    // Change color on hover
    sf::Vector2i mousePos = sf::Mouse::getPosition();
    sf::Vector2f mouseF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
    if (m_shape.getGlobalBounds().contains(mouseF)) {
        m_shape.setFillColor(sf::Color(80, 80, 80)); // lighter gray
    }
    else {
        m_shape.setFillColor(sf::Color(50, 50, 50));
    }

    // Make sure shape & text follow widget position/size:
    m_shape.setPosition(m_position);
    m_shape.setSize(m_size);

    auto bounds = m_text.getLocalBounds();
    m_text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
    m_text.setPosition(m_position + (m_size / 2.0f));
}

void Button::render(sf::RenderTarget& target) {
    if (!m_visible) return;
    target.draw(m_shape);
    target.draw(m_text);
}
>>>>>>> 70f8dadc4c8b82d7171402a7e9888286f78eafbe

#include "Button.h"

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
}

void Button::setFont(const sf::Font& font) {
    m_text.setFont(font);
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

#include "Button.h"

Button::Button()
    : m_interaction(m_model), m_renderer(m_model, m_interaction) {
}

Button::Button(const sf::Vector2f& position, const sf::Vector2f& size, const std::string& text)
    : m_interaction(m_model), m_renderer(m_model, m_interaction) {
    m_model.position = position;
    m_model.size = size;
    m_model.text = text;
}

void Button::setPosition(const sf::Vector2f& pos) {
    m_model.position = pos;
}

void Button::setSize(const sf::Vector2f& size) {
    m_model.size = size;
}

void Button::setText(const std::string& text) {
    m_model.text = text;
}

void Button::setFont(const sf::Font& font) {
    m_model.font = &font;
}

void Button::setTexture(const sf::Texture* texture) {
    m_model.texture = texture;
}

void Button::setColors(const sf::Color& bg, const sf::Color& text) {
    m_model.backgroundColor = bg;
    m_model.textColor = text;
}

void Button::setCallback(std::function<void()> callback) {
    m_interaction.setCallback(callback);
}

void Button::handleMouseMove(const sf::Vector2f& mousePos) {
    m_interaction.handleMouseMove(mousePos);
}

bool Button::handleClick(const sf::Vector2f& mousePos) {
    return m_interaction.handleClick(mousePos);
}

void Button::update(float deltaTime) {
    m_interaction.update(deltaTime);
}

void Button::render(sf::RenderWindow& window) {
    m_renderer.render(window);
}

void Button::setButtonImage(const sf::Texture* texture) {
    m_model.texture = texture;
}

void Button::setBackgroundColor(const sf::Color& color) {
    m_model.backgroundColor = color;
}

void Button::setTextColor(const sf::Color& color) {
    m_model.textColor = color;
}

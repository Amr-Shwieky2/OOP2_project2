#include "ButtonRenderer.h"

ButtonRenderer::ButtonRenderer(ButtonModel& model, ButtonInteraction& interaction)
    : m_model(model), m_interaction(interaction) {
    m_background.setOutlineColor(sf::Color::White);
    m_background.setOutlineThickness(2);

    m_text.setCharacterSize(24);
    m_text.setStyle(sf::Text::Bold);
}

void ButtonRenderer::render(sf::RenderWindow& window) {
    updateGraphics();

    float scale = m_interaction.getHoverScale();
    sf::Vector2f scaledSize = m_model.size * scale;
    sf::Vector2f offset = (scaledSize - m_model.size) * 0.5f;
    sf::Vector2f position = m_model.position - offset;

    m_background.setSize(scaledSize);
    m_background.setPosition(position);

    if (m_model.texture) {
        m_sprite.setScale(
            scaledSize.x / m_model.texture->getSize().x,
            scaledSize.y / m_model.texture->getSize().y
        );
        m_sprite.setPosition(position);
    }

    if (m_interaction.isHovered()) {
        sf::RectangleShape glow;
        glow.setSize(scaledSize + sf::Vector2f(20, 20));
        glow.setPosition(position - sf::Vector2f(10, 10));
        glow.setFillColor(sf::Color(255, 255, 255, 30));
        glow.setOutlineColor(sf::Color(255, 255, 0, 100));
        glow.setOutlineThickness(3);
        window.draw(glow);
    }

    if (m_model.texture) {
        window.draw(m_sprite);
    }
    else {
        window.draw(m_background);
    }

    window.draw(m_text);
}

void ButtonRenderer::updateGraphics() {
    m_background.setFillColor(m_model.backgroundColor);

    if (m_model.texture) {
        m_sprite.setTexture(*m_model.texture);
    }

    if (m_model.font) {
        m_text.setFont(*m_model.font);
    }

    m_text.setString(m_model.text);
    m_text.setFillColor(m_model.textColor);

    updateTextPosition();
}

void ButtonRenderer::updateTextPosition() {
    sf::FloatRect bounds = m_background.getGlobalBounds();
    sf::FloatRect textBounds = m_text.getLocalBounds();

    m_text.setPosition(
        bounds.left + (bounds.width - textBounds.width) / 2.f,
        bounds.top + (bounds.height - textBounds.height) / 2.f - textBounds.top
    );
}

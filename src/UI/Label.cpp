#include "Label.h"

Label::Label() {
    // Default: empty string
}

Label::~Label() {
}

void Label::setText(const std::string& str) {
    m_text.setString(str);
    // Update origin so position is top-left by default:
    m_text.setPosition(m_position);
}

void Label::setFont(const sf::Font& font) {
    m_text.setFont(font);
}

void Label::setCharacterSize(unsigned int size) {
    m_text.setCharacterSize(size);
}

void Label::setColor(const sf::Color& color) {
    m_text.setFillColor(color);
}

void Label::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    (void)event; (void)window;
    // Labels are not interactive by default
}

void Label::update(float deltaTime) {
    (void)deltaTime;
    // Ensure label’s position follows widget’s position:
    m_text.setPosition(m_position);
}

void Label::render(sf::RenderTarget& target) {
    if (!m_visible) return;
    target.draw(m_text);
}

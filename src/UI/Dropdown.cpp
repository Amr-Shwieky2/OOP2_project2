#include "Dropdown.h"

Dropdown::Dropdown(sf::Vector2f position, sf::Vector2f size, const sf::Font& font)
    : m_position(position), m_size(size), m_font(font) {

    m_background.setPosition(position);
    m_background.setSize(size);
    m_background.setFillColor(m_backgroundColor);
    m_background.setOutlineThickness(2);
    m_background.setOutlineColor(sf::Color(100, 100, 100));

    m_selectedText.setFont(font);
    m_selectedText.setCharacterSize(20);
    m_selectedText.setFillColor(m_textColor);
    m_selectedText.setPosition(position.x + 10, position.y + 5);

    m_arrowText.setFont(font);
    m_arrowText.setString("v");
    m_arrowText.setCharacterSize(16);
    m_arrowText.setFillColor(m_textColor);
    m_arrowText.setPosition(position.x + size.x - 25, position.y + 8);
}

void Dropdown::addOption(const std::string& text, int value) {
    m_options.push_back({ text, value });
    if (m_selectedIndex == -1) {
        setSelectedIndex(0);
    }
}

void Dropdown::setSelectedIndex(int index) {
    if (index >= 0 && index < static_cast<int>(m_options.size())) {
        int oldIndex = m_selectedIndex;
        m_selectedIndex = index;
        updateSelectedText();

        if (m_onSelectionChanged && oldIndex != index) {
            m_onSelectionChanged(index, m_options[index].value);
        }
    }
}

int Dropdown::getSelectedValue() const {
    if (m_selectedIndex >= 0 && m_selectedIndex < static_cast<int>(m_options.size())) {
        return m_options[m_selectedIndex].value;
    }
    return -1;
}

bool Dropdown::handleMouseMove(sf::Vector2f mousePos) {
    if (m_isOpen) {
        m_hoveredIndex = -1;
        for (int i = 0; i < static_cast<int>(m_options.size()); ++i) {
            if (getOptionBounds(i).contains(mousePos)) {
                m_hoveredIndex = i;
                break;
            }
        }
        return true;
    }
    else {
        return getMainBounds().contains(mousePos);
    }
}

bool Dropdown::handleMousePressed(sf::Vector2f mousePos) {
    if (m_isOpen) {
        if (m_hoveredIndex >= 0) {
            setSelectedIndex(m_hoveredIndex);
            m_isOpen = false;
            return true;
        }
        else {
            m_isOpen = false;
            return false;
        }
    }
    else {
        if (getMainBounds().contains(mousePos)) {
            m_isOpen = true;
            return true;
        }
    }
    return false;
}

void Dropdown::update(float deltaTime) {
    // Animation could be added here
}

void Dropdown::render(sf::RenderWindow& window) {
    // Draw main dropdown
    window.draw(m_background);
    window.draw(m_selectedText);
    window.draw(m_arrowText);

    // Draw options if open
    if (m_isOpen) {
        for (int i = 0; i < static_cast<int>(m_options.size()); ++i) {
            sf::RectangleShape optionBg;
            optionBg.setPosition(m_position.x, m_position.y + m_size.y + (i * 30));
            optionBg.setSize(sf::Vector2f(m_size.x, 30));

            if (i == m_hoveredIndex) {
                optionBg.setFillColor(m_highlightColor);
            }
            else {
                optionBg.setFillColor(m_backgroundColor);
            }
            optionBg.setOutlineThickness(1);
            optionBg.setOutlineColor(sf::Color(100, 100, 100));

            window.draw(optionBg);

            sf::Text optionText;
            optionText.setFont(m_font);
            optionText.setString(m_options[i].text);
            optionText.setCharacterSize(20);
            optionText.setFillColor(m_textColor);
            optionText.setPosition(m_position.x + 10, m_position.y + m_size.y + (i * 30) + 5);

            window.draw(optionText);
        }
    }
}

void Dropdown::setColors(sf::Color background, sf::Color text, sf::Color highlight) {
    m_backgroundColor = background;
    m_textColor = text;
    m_highlightColor = highlight;

    m_background.setFillColor(m_backgroundColor);
    m_selectedText.setFillColor(m_textColor);
    m_arrowText.setFillColor(m_textColor);
}

void Dropdown::updateSelectedText() {
    if (m_selectedIndex >= 0 && m_selectedIndex < static_cast<int>(m_options.size())) {
        m_selectedText.setString(m_options[m_selectedIndex].text);
    }
}

sf::FloatRect Dropdown::getOptionBounds(int index) const {
    return sf::FloatRect(
        m_position.x,
        m_position.y + m_size.y + (index * 30),
        m_size.x,
        30
    );
}

sf::FloatRect Dropdown::getMainBounds() const {
    return sf::FloatRect(m_position.x, m_position.y, m_size.x, m_size.y);
}
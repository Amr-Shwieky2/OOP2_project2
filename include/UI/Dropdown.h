#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <functional>

class Dropdown {
public:
    Dropdown(sf::Vector2f position, sf::Vector2f size, const sf::Font& font);

    void addOption(const std::u8string& text, int value);
    void setSelectedIndex(int index);
    int getSelectedIndex() const { return m_selectedIndex; }
    int getSelectedValue() const;

    void setOnSelectionChanged(std::function<void(int, int)> callback) { m_onSelectionChanged = callback; }

    bool handleMouseMove(sf::Vector2f mousePos);
    bool handleMousePressed(sf::Vector2f mousePos);

    void update(float deltaTime);
    void render(sf::RenderWindow& window);

    void setColors(sf::Color background, sf::Color text, sf::Color highlight);

    bool updateSelectedTextSafely();

    bool renderOptionTextSafely(sf::RenderWindow& window, int index);

private:
    struct Option {
        std::u8string text;
        int value;
    };

    sf::Vector2f m_position;
    sf::Vector2f m_size;
    const sf::Font& m_font;

    std::vector<Option> m_options;
    int m_selectedIndex = -1;
    int m_hoveredIndex = -1;
    bool m_isOpen = false;

    sf::RectangleShape m_background;
    sf::Text m_selectedText;
    sf::Text m_arrowText;

    sf::Color m_backgroundColor = sf::Color(70, 70, 70);
    sf::Color m_textColor = sf::Color::White;
    sf::Color m_highlightColor = sf::Color(100, 150, 255);

    std::function<void(int, int)> m_onSelectionChanged;

    void updateSelectedText();
    sf::FloatRect getOptionBounds(int index) const;
    sf::FloatRect getMainBounds() const;
};
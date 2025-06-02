#pragma once

#include <SFML/Graphics.hpp>

///
/// Widget (abstract)
/// -----------------
/// Base for any UI widget. Provides position, size, visibility, and enabled/disabled state.
/// Derived classes override handleEvent(), update(), render().
///
class Widget {
public:
    Widget();
    virtual ~Widget();

    // Called on each SFML Event (mouse, keyboard, etc.)
    virtual void handleEvent(const sf::Event& event, sf::RenderWindow& window) = 0;

    // Called each frame, if visible, to update animations or state
    virtual void update(float deltaTime) = 0;

    // Called each frame to draw the widget if visible
    virtual void render(sf::RenderTarget& target) = 0;

    // Position and size setters/getters
    void setPosition(const sf::Vector2f& pos);
    sf::Vector2f getPosition() const;

    void setSize(const sf::Vector2f& s);
    sf::Vector2f getSize() const;

    // Show or hide the widget
    void setVisible(bool v);
    bool isVisible() const;

    // Enable or disable user interaction
    void setEnabled(bool e);
    bool isEnabled() const;

protected:
    sf::Vector2f m_position;
    sf::Vector2f m_size;
    bool m_visible;
    bool m_enabled;
};

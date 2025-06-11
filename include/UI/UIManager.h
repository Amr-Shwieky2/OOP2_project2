#pragma once

#include <vector>
#include <memory>
#include "Widget.h"
#include <SFML/Graphics.hpp>

///
/// UIManager
/// ---------
/// Keeps a list of all active UI widgets, forwards events, updates, and renders them.
///
class UIManager {
public:
    UIManager();
    ~UIManager();

    // Add a widget to be managed (e.g. Button, Label, HUD, etc.)
    void addWidget(std::shared_ptr<Widget> widget);

    // Remove a widget (optional; not used right away)
    void removeWidget(std::shared_ptr<Widget> widget);

    // Forward SFML Event to each visible/enabled widget
    void handleEvent(const sf::Event& event, sf::RenderWindow& window);

    // Call update(deltaTime) on each visible widget
    void update(float deltaTime);

    // Call render(window) on each visible widget
    void render(sf::RenderTarget& target);

private:
    std::vector<std::shared_ptr<Widget>> m_widgets;
};

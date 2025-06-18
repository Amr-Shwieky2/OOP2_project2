#pragma once

#include <SFML/Graphics.hpp>

// Abstract interface for all screen types
class IScreen {
public:
    virtual ~IScreen() = default;

    // Handle SFML input/events
    virtual void handleEvents(sf::RenderWindow& window) = 0;

    // Update logic per frame
    virtual void update(float deltaTime) = 0;

    // Render visual elements to the window
    virtual void render(sf::RenderWindow& window) = 0;
};

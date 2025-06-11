#pragma once

#include <SFML/Graphics.hpp>

// IScreen defines the minimal interface that every “screen” (menu, gameplay, pause, etc.)
// must implement. Because of the pure‐virtual methods, IScreen is an abstract class.
class IScreen {
public:
    virtual ~IScreen() = default;

    // Handle any input events (keyboard, mouse, window close, etc.)
    virtual void handleInput(sf::RenderWindow& window) = 0;

    // Advance the screen’s internal logic by deltaTime seconds
    virtual void update(float deltaTime) = 0;

    // Draw this screen’s contents to the provided render window
    virtual void render(sf::RenderWindow& window) = 0;

    // Called when this screen becomes active (pushed onto the stack or just switched to)
    virtual void onEnter() = 0;

    // Called when this screen is about to be removed (popped or replaced)
    virtual void onExit() = 0;
};

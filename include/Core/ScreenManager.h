#pragma once

#include <stack>
#include <memory>
#include "IScreen.h"
#include <SFML/Graphics.hpp>

// ScreenManager is responsible for holding a stack of screens.
// You can push a new screen (pausing the old one), pop the current screen,
// or replace the current screen entirely.
class ScreenManager {
public:
    ScreenManager();
    ~ScreenManager();

    // Push a brand‐new screen on top of the stack.
    // The previous screen (if any) will receive onExit(),
    // and then this new screen will receive onEnter().
    void pushScreen(std::unique_ptr<IScreen> screen);

    // Pop (remove) the topmost screen.
    // The top screen receives onExit(); then, if there’s a new top, it receives onEnter().
    void popScreen();

    // Replace the current top screen with a brand‐new one.
    // The old top receives onExit(), is removed, and the new one receives onEnter().
    void changeScreen(std::unique_ptr<IScreen> screen);

    // Return a pointer to the active (top) screen, or nullptr if none.
    IScreen* getActiveScreen();

    // Forward the SFML window to the active screen for input handling
    void handleInput(sf::RenderWindow& window);

    // Forward update/deltaTime to the active screen
    void update(float deltaTime);

    // Forward render call to the active screen
    void render(sf::RenderWindow& window);

private:
    // A stack of screens. The top of the stack is the currently active screen.
    std::stack<std::unique_ptr<IScreen>> m_screens;
};

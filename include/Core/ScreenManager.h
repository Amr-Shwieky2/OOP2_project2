#pragma once

#include <memory>
#include <unordered_map>
#include <functional>
#include "IScreen.h"
#include "ScreenTypes.h"  // You’ll define enum ScreenType there

// Manages current screen and transitions between screens
class ScreenManager {
public:
    // Register a screen factory for a screen type
    void registerScreen(ScreenType type, std::function<std::unique_ptr<IScreen>()> creator);

    // Change to a new screen
    void changeScreen(ScreenType type);

    // Forward to current screen
    void handleEvents(sf::RenderWindow& window);
    void update(float deltaTime);
    void render(sf::RenderWindow& window);

private:
    std::unordered_map<ScreenType, std::function<std::unique_ptr<IScreen>()>> m_creators;
    std::unique_ptr<IScreen> m_currentScreen;
};

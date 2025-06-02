#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "RandomService.h"
// Include each service’s header here (so we can construct them)
#include "ScreenManager.h"
#include "AudioService.h"
#include "ResourceLoader.h"
#include "InputService.h"


///
/// AppContext
/// -----------
/// This is a “service locator” / dependency‐injection container for the entire game.
/// It constructs and holds ownership of all long‐lived services:
///  • ScreenManager      (manages current screen)
///  • AudioService       (plays SFX & music)
///  • ResourceLoader     (loads textures/fonts, caches them)
///  • InputService       (reads keyboard/controller state)
///  • RandomService      (random numbers for AI, relics, etc.)
///  • Box2D b2World      (physics world for collision, gravity)
///
/// Anything that needs access to these services can be given an `AppContext&`
/// (or a pointer/reference to it) and call the `getXxx()` methods below.
///
/// Note:
///  • We pass the SFML window to AudioService (if needed) and InputService as references.
///  • We create the Box2D world here so that any physics‐enabled entity can retrieve it.
///

class AppContext {
public:
    // Constructor takes the SFML window reference, so services can use it if needed
    explicit AppContext(sf::RenderWindow& mainWindow);

    // Destructor: default is fine; unique_ptr will free resources automatically
    ~AppContext() = default;

    // No copying
    AppContext(const AppContext&) = delete;
    AppContext& operator=(const AppContext&) = delete;

    // Accessors for each service:
    ScreenManager& getScreenManager();
    AudioService& getAudioService();
    ResourceLoader& getResourceLoader();
    InputService& getInputService();
    RandomService& getRandomService();
    b2World& getPhysicsWorld();

private:
    // The main SFML window (not owned, just referenced). Many services will need it.
    sf::RenderWindow& m_window;

    // Box2D physics world. We choose “−9.8f” gravity in the y‐direction.
    std::unique_ptr<b2World> m_physicsWorld;

    // All the major services the game needs, each owned by AppContext.
    std::unique_ptr<ScreenManager>   m_screenManager;
    std::unique_ptr<AudioService>    m_audioService;
    std::unique_ptr<ResourceLoader>  m_resourceLoader;
    std::unique_ptr<InputService>    m_inputService;
    std::unique_ptr<RandomService>   m_randomService;
};

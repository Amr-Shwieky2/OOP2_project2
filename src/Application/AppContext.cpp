#include "AppContext.h"

// Include each service’s header here (so we can construct them)
#include "ScreenManager.h"
#include "AudioService.h"
#include "ResourceLoader.h"
#include "InputService.h"
#include "RandomService.h"

///
/// AppContext Implementation
///

AppContext::AppContext(sf::RenderWindow& mainWindow)
    : m_window(mainWindow)
{
    // 1) Create the Box2D world with standard gravity (downwards).
    //    We use a pointer because Box2D’s b2World has no default constructor.
    m_physicsWorld = std::make_unique<b2World>(b2Vec2(0.0f, -9.8f));

    // 2) Construct each service in turn, passing references if required:

    // 2a) ScreenManager: manages pushing/popping screens
    m_screenManager = std::make_unique<ScreenManager>();

    // 2b) AudioService: needs the SFML window if it plays music in a background thread
    //     (Even if it doesn’t strictly need the window, having it accessible can help.)
    m_audioService = std::make_unique<AudioService>(/* If you need window reference, add 'm_window' here */);

    // 2c) ResourceLoader: loads textures, fonts, sounds on‐demand and caches them
    m_resourceLoader = std::make_unique<ResourceLoader>();

    // 2d) InputService: polls keyboard/mouse; needs window reference so it can check events if desired
    m_inputService = std::make_unique<InputService>(m_window);

    // 2e) RandomService: simple RNG class; no extra parameters
    m_randomService = std::make_unique<RandomService>();

    // At this point, every major service is constructed and ready to use.
}

ScreenManager& AppContext::getScreenManager() {
    return *m_screenManager;
}

AudioService& AppContext::getAudioService() {
    return *m_audioService;
}

ResourceLoader& AppContext::getResourceLoader() {
    return *m_resourceLoader;
}

InputService& AppContext::getInputService() {
    return *m_inputService;
}

RandomService& AppContext::getRandomService() {
    return *m_randomService;
}

b2World& AppContext::getPhysicsWorld() {
    return *m_physicsWorld;
}

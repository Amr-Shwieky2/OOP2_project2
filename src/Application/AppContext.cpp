#include "AppContext.h"

// Singleton instance getter - creates static instance on first call
AppContext& AppContext::instance() {
    // Static local variable ensures single instance creation
    // Thread-safe in C++11 and later standards
    static AppContext context;
    return context;
}

// Private constructor - initializes all services using smart pointers
AppContext::AppContext() {
    // Create unique instances of each service
    // Order of initialization may be important for service dependencies
    m_audioService = std::make_unique<AudioService>();
    m_resourceLoader = std::make_unique<ResourceLoader>();
    m_screenManager = std::make_unique<ScreenManager>();

    // Initialize resource managers for specific asset types
    m_textureManager = std::make_unique<TextureManager>();
    m_fontManager = std::make_unique<FontManager>();
    m_soundManager = std::make_unique<SoundManager>();
}

// Service accessor implementations - return dereferenced smart pointers
// These methods provide direct access to service instances

AudioService& AppContext::audio() {
    // Dereference the unique_ptr to return a reference to the AudioService
    return *m_audioService;
}

ResourceLoader& AppContext::resources() {
    // Dereference to provide access to general resource loading functionality
    return *m_resourceLoader;
}

ScreenManager& AppContext::screenManager() {
    // Dereference to provide access to screen management system
    return *m_screenManager;
}

TextureManager& AppContext::textures() {
    // Dereference to provide access to texture/image management
    return *m_textureManager;
}

FontManager& AppContext::fonts() {
    // Dereference to provide access to font resource management
    return *m_fontManager;
}

SoundManager& AppContext::sounds() {
    // Dereference to provide access to sound effect management
    return *m_soundManager;
}
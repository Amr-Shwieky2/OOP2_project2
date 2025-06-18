/**
 * @brief Central application context using Singleton pattern for service management
 *
 * AppContext provides a single access point to all application services and manages
 * their lifecycle. It ensures only one instance of each service exists and handles
 * automatic memory management using smart pointers.
 *
 * Services managed:
 * - AudioService: Audio and music management
 * - ResourceLoader: General resource loading
 * - ScreenManager: Screen and UI management
 * - TextureManager: Image and texture management
 * - FontManager: Font resource management
 * - SoundManager: Sound effect management
 *
 * Usage: AppContext::instance().serviceName().method()
 */
#pragma once
#include <memory>
#include "AudioService.h"
#include "ResourceLoader.h"
#include "ScreenManager.h"
#include "ResourceManager.h"  

class AppContext {
public:
    static AppContext& instance();
    AudioService& audio();
    ResourceLoader& resources();
    ScreenManager& screenManager();

    //Resource Managers only
    TextureManager& textures();
    FontManager& fonts();
    SoundManager& sounds();

private:
    AppContext();
    AppContext(const AppContext&) = delete;
    AppContext& operator=(const AppContext&) = delete;

    // Existing services (unchanged)
    std::unique_ptr<AudioService> m_audioService;
    std::unique_ptr<ResourceLoader> m_resourceLoader;
    std::unique_ptr<ScreenManager> m_screenManager;

    // Resource Managers only
    std::unique_ptr<TextureManager> m_textureManager;
    std::unique_ptr<FontManager> m_fontManager;
    std::unique_ptr<SoundManager> m_soundManager;
};
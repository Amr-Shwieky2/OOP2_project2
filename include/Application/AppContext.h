// include/Application/AppContext.h
#pragma once

#include <memory>
#include "AudioService.h"
#include "ResourceLoader.h"
#include "ScreenManager.h"
#include "ResourceManager.h"  

class AppContext {
public:
    static AppContext& instance();

    // الخدمات الموجودة (بدون تغيير)
    AudioService& audio();
    ResourceLoader& resources();
    ScreenManager& screenManager();

    // ✅ Template Resource Managers الجديدة فقط
    TextureManager& textures();
    FontManager& fonts();
    SoundManager& sounds();

private:
    AppContext();
    AppContext(const AppContext&) = delete;
    AppContext& operator=(const AppContext&) = delete;

    // الخدمات الموجودة (بدون تغيير)
    std::unique_ptr<AudioService> m_audioService;
    std::unique_ptr<ResourceLoader> m_resourceLoader;
    std::unique_ptr<ScreenManager> m_screenManager;

    // ✅ Template Resource Managers الجديدة فقط
    std::unique_ptr<TextureManager> m_textureManager;
    std::unique_ptr<FontManager> m_fontManager;
    std::unique_ptr<SoundManager> m_soundManager;
};
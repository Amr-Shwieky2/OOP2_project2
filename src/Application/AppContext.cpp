﻿#include "AppContext.h"

AppContext & AppContext::instance() {
    static AppContext context;
    return context;
}

AppContext::AppContext() {
    // الخدمات الموجودة
    m_audioService = std::make_unique<AudioService>();
    m_resourceLoader = std::make_unique<ResourceLoader>();
    m_screenManager = std::make_unique<ScreenManager>();

    // ✅ Template Resource Managers الجديدة
    m_textureManager = std::make_unique<TextureManager>();
    m_fontManager = std::make_unique<FontManager>();
    m_soundManager = std::make_unique<SoundManager>();
}

// الخدمات الموجودة
AudioService& AppContext::audio() {
    return *m_audioService;
}

ResourceLoader& AppContext::resources() {
    return *m_resourceLoader;
}

ScreenManager& AppContext::screenManager() {
    return *m_screenManager;
}

// ✅ Template Resource Managers الجديدة
TextureManager& AppContext::textures() {
    return *m_textureManager;
}

FontManager& AppContext::fonts() {
    return *m_fontManager;
}

SoundManager& AppContext::sounds() {
    return *m_soundManager;
}
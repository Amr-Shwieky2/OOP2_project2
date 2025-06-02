#include "AppContext.h"

AppContext& AppContext::instance() {
    static AppContext context;
    return context;
}

AppContext::AppContext() {
    m_audioService = std::make_unique<AudioService>();
    m_resourceLoader = std::make_unique<ResourceLoader>();
    m_screenManager = std::make_unique<ScreenManager>(); 
}

AudioService& AppContext::audio() {
    return *m_audioService;
}

ResourceLoader& AppContext::resources() {
    return *m_resourceLoader;
}

ScreenManager& AppContext::screenManager() {
    return *m_screenManager; // ✅ Accessor
}

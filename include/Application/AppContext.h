#pragma once

#include <memory>
#include "AudioService.h"
#include "ResourceLoader.h"
#include "ScreenManager.h"

class AppContext {
public:
    static AppContext& instance();

    AudioService& audio();
    ResourceLoader& resources();
    ScreenManager& screenManager(); 

private:
    AppContext();
    AppContext(const AppContext&) = delete;
    AppContext& operator=(const AppContext&) = delete;

    std::unique_ptr<AudioService> m_audioService;
    std::unique_ptr<ResourceLoader> m_resourceLoader;
    std::unique_ptr<ScreenManager> m_screenManager; // ✅ New member
};

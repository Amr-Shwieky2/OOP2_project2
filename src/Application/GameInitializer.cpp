#include "GameInitializer.h"
#include "../Core/SmartExceptions.h"

using namespace SmartExceptions;

void GameInitializer::initializeAllSystems() {
    Logger::log("Starting game systems initialization...");

    SAFE_EXECUTE(
        {
            initializeAudioSystem();
            initializeResourceSystem();
            registerAllScreens();

            Logger::log("All game systems initialized successfully");
        },
        "initializeAllSystems",
        "GameInitializer",
        Category::System
    );
}

void GameInitializer::initializeAudioSystem() {
    SAFE_EXECUTE(
        {
            Logger::log("Initializing audio system...");

            AudioSettings settings;
            if (AudioSettingsManager::load(settings)) {
                AudioManager::instance().setMasterVolume(settings.masterVolume);
                AudioManager::instance().setMusicVolume(settings.musicVolume);
                AudioManager::instance().setSFXVolume(settings.sfxVolume);

                Logger::log("Audio settings loaded successfully");
            }
            else {
                Logger::log("Using default audio settings", LogLevel::Warning);
            }

            loadDefaultAudioFiles();
            setDefaultAudioVolumes();

            Logger::log("Audio system initialized successfully");
        },
        "initializeAudioSystem",
        "GameInitializer",
        Category::Audio
    );
}

void GameInitializer::loadDefaultAudioFiles() {
    SAFE_EXECUTE(
        {
            auto& audioManager = AudioManager::instance();

            if (audioManager.loadMusic("loading_music", "intro.wav")) {
                Logger::log("Loading music loaded successfully");
            }
            else {
                THROW_AUDIO_EXCEPTION(
                    "Could not load loading music file: intro.wav",
                    Severity::Warning,
                    RecoveryStrategy::Fallback
                );
            }
        },
        "loadDefaultAudioFiles",
        "GameInitializer",
        Category::Audio
    );
}

void GameInitializer::setDefaultAudioVolumes() {
    SAFE_EXECUTE(
        {
            AudioSettings settings;
            settings.masterVolume = AudioManager::instance().getMasterVolume();
            settings.musicVolume = AudioManager::instance().getMusicVolume();
            settings.sfxVolume = AudioManager::instance().getSFXVolume();

            AudioSettingsManager::save(settings);
            Logger::log("Set default audio volumes");
        },
        "setDefaultAudioVolumes",
        "GameInitializer",
        Category::Configuration
    );
}

void GameInitializer::initializeResourceSystem() {
    SAFE_EXECUTE(
        {
            Logger::log("Initializing resource system...");

            try {
                auto& textures = AppContext::instance().textures();
                auto& fonts = AppContext::instance().fonts();
                auto& sounds = AppContext::instance().sounds();

                Logger::log("Resource system initialized successfully");
            }
            catch (const std::exception& e) {
                THROW_RESOURCE_EXCEPTION(
                    "Failed to initialize resource system: " + std::string(e.what()),
                    Severity::Critical,
                    RecoveryStrategy::None
                );
            }
        },
        "initializeResourceSystem",
        "GameInitializer",
        Category::Resource
    );
}

void GameInitializer::registerAllScreens() {
    SAFE_EXECUTE(
        {
            Logger::log("Registering all screens...");
            
            try {
                registerScreenFactories();
                AppContext::instance().screenManager().changeScreen(ScreenType::LOADING);
                Logger::log("All screens registered successfully");
            }
            catch (const std::exception& e) {
                THROW_UI_EXCEPTION(
                    "Failed to register screens: " + std::string(e.what()),
                    Severity::Critical,
                    RecoveryStrategy::None
                );
            }
        },
        "registerAllScreens",
        "GameInitializer",
        Category::UI
    );
}

void GameInitializer::registerScreenFactories() {
    SAFE_EXECUTE(
        {
            auto& screenManager = AppContext::instance().screenManager();

            screenManager.registerScreen(ScreenType::LOADING, []() {
                return std::make_unique<LoadingScreen>();
            });

            screenManager.registerScreen(ScreenType::MENU, []() {
                return std::make_unique<MenuScreen>();
            });

            screenManager.registerScreen(ScreenType::SETTINGS, []() {
                return std::make_unique<SettingsScreen>();
            });

            screenManager.registerScreen(ScreenType::HELP, []() {
                return std::make_unique<HelpScreen>();
            });

            screenManager.registerScreen(ScreenType::ABOUT_US, []() {
                return std::make_unique<AboutScreen>();
            });
        },
        "registerScreenFactories",
        "GameInitializer",
        Category::UI
    );
}

void GameInitializer::handleInitializationError(const std::string& system, const std::string& error) {
    // This method is now deprecated in favor of smart exception system
    Logger::log("Error initializing " + system + ": " + error, LogLevel::Error);
}

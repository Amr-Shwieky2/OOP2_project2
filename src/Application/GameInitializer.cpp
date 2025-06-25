#include "GameInitializer.h"
#include "AppContext.h"
#include "ScreenTypes.h"
#include "../Core/AudioManager.h"
#include "../Core/AudioSettingsManager.h"
#include "../Core/MenuSoundManager.h"
#include "Logger.h"

// Screens
#include "../Screens/LoadingScreen.h"
#include "../Screens/MenuScreen.h"
#include "../Screens/SettingsScreen.h"
#include "../Screens/AboutScreen.h"
#include "../Screens/HelpScreen.h"

void GameInitializer::initializeAllSystems() {
    Logger::log("Starting game systems initialization...");

    try {
        initializeAudioSystem();
        initializeResourceSystem();
        registerAllScreens();

        Logger::log("All game systems initialized successfully");
    }
    catch (const std::exception& e) {
        Logger::log("Critical initialization failure: " + std::string(e.what()), LogLevel::Error);
        throw;
    }
}

void GameInitializer::initializeAudioSystem() {
    try {
        Logger::log("Initializing audio system...");

        AudioSettings settings;
        if (AudioSettingsManager::load(settings)) {
            AudioManager::instance().setMasterVolume(settings.masterVolume);
            AudioManager::instance().setMusicVolume(settings.musicVolume);
            AudioManager::instance().setSFXVolume(settings.sfxVolume);

            MenuSoundManager::instance().enable(settings.menuSoundsEnabled);
            MenuSoundManager::instance().setVolume(settings.menuSoundVolume);

            Logger::log("Audio settings loaded successfully");
        }
        else {
            Logger::log("Using default audio settings", LogLevel::Warning);
        }

        loadDefaultAudioFiles();
        setDefaultAudioVolumes();

        Logger::log("Audio system initialized successfully");
    }
    catch (const std::exception& e) {
        handleInitializationError("Audio System", e.what());
        Logger::log("Game will continue without audio", LogLevel::Warning);
    }
}

void GameInitializer::loadDefaultAudioFiles() {
    auto& audioManager = AudioManager::instance();
    auto& menuSounds = MenuSoundManager::instance();

    if (menuSounds.loadSounds()) {
        Logger::log("Menu sounds loaded successfully");
    }
    else {
        Logger::log("Warning: Could not load menu sounds", LogLevel::Warning);
    }

    if (audioManager.loadMusic("background_music", "background_music.ogg")) {
        Logger::log("Background music loaded successfully");
    }
    else {
        Logger::log("Warning: Could not load background music", LogLevel::Warning);
    }
}

void GameInitializer::setDefaultAudioVolumes() {
    AudioSettings settings;
    settings.masterVolume = AudioManager::instance().getMasterVolume();
    settings.musicVolume = AudioManager::instance().getMusicVolume();
    settings.sfxVolume = AudioManager::instance().getSFXVolume();
    settings.menuSoundsEnabled = MenuSoundManager::instance().isEnabled();
    settings.menuSoundVolume = MenuSoundManager::instance().getVolume();

    AudioSettingsManager::save(settings);
    Logger::log("Set default audio volumes");
}

void GameInitializer::initializeResourceSystem() {
    try {
        Logger::log("Initializing resource system...");

        auto& textures = AppContext::instance().textures();
        auto& fonts = AppContext::instance().fonts();
        auto& sounds = AppContext::instance().sounds();

        Logger::log("Resource system initialized successfully");
    }
    catch (const std::exception& e) {
        handleInitializationError("Resource System", e.what());
        throw;
    }
}

void GameInitializer::registerAllScreens() {
    try {
        Logger::log("Registering all screens...");
        registerScreenFactories();
        AppContext::instance().screenManager().changeScreen(ScreenType::LOADING);
        Logger::log("All screens registered successfully");
    }
    catch (const std::exception& e) {
        handleInitializationError("Screen System", e.what());
        throw;
    }
}

void GameInitializer::registerScreenFactories() {
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
}

void GameInitializer::handleInitializationError(const std::string& system, const std::string& error) {
    Logger::log("Error initializing " + system + ": " + error, LogLevel::Error);
}

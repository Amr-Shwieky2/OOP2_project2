#include "GameInitializer.h"
#include "AppContext.h"
#include "ScreenTypes.h"
#include "AudioManager.h"
#include "Logger.h"

// Screen includes
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

        auto& audioManager = AudioManager::instance();

        // Load audio settings first
        audioManager.loadSettings();

        // Load audio files
        loadDefaultAudioFiles();

        // Set default volumes if needed
        setDefaultAudioVolumes();

        Logger::log("Audio system initialized successfully");
    }
    catch (const std::exception& e) {
        // FIXED: Use string instead of exception object
        handleInitializationError("Audio System", e.what());
        // Continue without audio rather than crash
        Logger::log("Game will continue without audio", LogLevel::Warning);
    }
}

void GameInitializer::initializeResourceSystem() {
    try {
        Logger::log("Initializing resource system...");

        // Resource loaders are created automatically by AppContext
        // Just validate they're working
        auto& textures = AppContext::instance().textures();
        auto& fonts = AppContext::instance().fonts();
        auto& sounds = AppContext::instance().sounds();

        Logger::log("Resource system initialized successfully");
    }
    catch (const std::exception& e) {
        // FIXED: Use string instead of exception object
        handleInitializationError("Resource System", e.what());
        throw; // Resource system is critical
    }
}

void GameInitializer::registerAllScreens() {
    try {
        Logger::log("Registering all screens...");

        registerScreenFactories();

        // Start with loading screen
        auto& screenManager = AppContext::instance().screenManager();
        screenManager.changeScreen(ScreenType::LOADING);

        Logger::log("All screens registered successfully");
    }
    catch (const std::exception& e) {
        // FIXED: Use string instead of exception object
        handleInitializationError("Screen System", e.what());
        throw; // Screen system is critical
    }
}

void GameInitializer::loadDefaultAudioFiles() {
    auto& audioManager = AudioManager::instance();

    // Load menu sounds (non-critical)
    if (audioManager.loadMenuSounds()) {
        Logger::log("Menu sounds loaded successfully");
    }
    else {
        Logger::log("Warning: Could not load menu sounds", LogLevel::Warning);
    }

    // Load background music (non-critical)
    if (audioManager.loadMusic("background_music", "background_music.ogg")) {
        Logger::log("Background music loaded successfully");
    }
    else {
        Logger::log("Warning: Could not load background music", LogLevel::Warning);
    }
}

void GameInitializer::setDefaultAudioVolumes() {
    auto& audioManager = AudioManager::instance();

    // Set default volumes if not loaded from settings
    if (audioManager.getMasterVolume() == 0) {
        audioManager.setMasterVolume(80.0f);
        audioManager.setSFXVolume(70.0f);
        audioManager.setMusicVolume(60.0f);
        Logger::log("Set default audio volumes");
    }
}

void GameInitializer::registerScreenFactories() {
    auto& screenManager = AppContext::instance().screenManager();

    // Register screen factory functions
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

// FIXED: Correct function signature and implementation
void GameInitializer::handleInitializationError(const std::string& system, const std::string& error) {
    Logger::log("Error initializing " + system + ": " + error, LogLevel::Error);
}
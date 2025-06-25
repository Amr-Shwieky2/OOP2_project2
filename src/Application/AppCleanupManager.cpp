#include "AppCleanupManager.h"
#include "AudioManager.h"
#include "AppContext.h"
#include "Logger.h"
#include <AudioSettingsManager.h>

void AppCleanupManager::performCleanup() {
    Logger::log("Starting application cleanup...");

    // Cleanup operations in safe order
    cleanupAudioSystem();
    saveUserSettings();
    cleanupResources();
    cleanupTempFiles();

    Logger::log("Application cleanup completed");
}

void AppCleanupManager::cleanupAudioSystem() {
    try {
        Logger::log("Cleaning up audio system...");

        auto& audioManager = AudioManager::instance();

        // Stop all audio playback
        audioManager.stopAllSounds();
        audioManager.stopMusic();

        logCleanupOperation("Audio system cleanup", true);
    }
    catch (const std::exception& e) {
        handleCleanupError("Audio system cleanup", e);
    }
}

void AppCleanupManager::saveUserSettings() {
    try {
        Logger::log("Saving user settings...");

        // Save audio settings from AudioManager to file
        auto& audioManager = AudioManager::instance();

        AudioSettings settings;
        settings.masterVolume = audioManager.getMasterVolume();
        settings.musicVolume = audioManager.getMusicVolume();
        settings.sfxVolume = audioManager.getSFXVolume();

        AudioSettingsManager::save(settings);

        logCleanupOperation("User settings save", true);
    }
    catch (const std::exception& e) {
        handleCleanupError("User settings save", e);
    }
}

void AppCleanupManager::cleanupResources() {
    try {
        Logger::log("Cleaning up resources...");

        // Resource managers clean up automatically via RAII
        // But we can log the cleanup for debugging
        auto& context = AppContext::instance();

        // Get resource counts for logging
        size_t textureCount = context.textures().getResourceCount();
        size_t fontCount = context.fonts().getResourceCount();
        size_t soundCount = context.sounds().getResourceCount();

        Logger::log("Resources cleaned up - Textures: " + std::to_string(textureCount) +
            ", Fonts: " + std::to_string(fontCount) +
            ", Sounds: " + std::to_string(soundCount));

        logCleanupOperation("Resource cleanup", true);
    }
    catch (const std::exception& e) {
        handleCleanupError("Resource cleanup", e);
    }
}

void AppCleanupManager::cleanupTempFiles() {
    try {
        Logger::log("Cleaning up temporary files...");

        // Future: Clean up any temporary files created during runtime
        // - Log files (if rotation is needed)
        // - Cache files
        // - Temporary save files

        logCleanupOperation("Temporary files cleanup", true);
    }
    catch (const std::exception& e) {
        handleCleanupError("Temporary files cleanup", e);
    }
}

void AppCleanupManager::handleCleanupError(const std::string& operation, const std::exception& e) {
    Logger::log("Error during " + operation + ": " + e.what(), LogLevel::Warning);
    logCleanupOperation(operation, false);

    // Continue cleanup even if one operation fails
    // Cleanup should be as robust as possible
}

void AppCleanupManager::logCleanupOperation(const std::string& operation, bool success) {
    std::string status = success ? "SUCCESS" : "FAILED";
    Logger::log("Cleanup operation: " + operation + " - " + status);
}
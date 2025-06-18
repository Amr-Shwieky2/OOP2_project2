#include "AudioManager.h"
#include <fstream>
#include <iostream>
#include <algorithm>

// Singleton instance getter
AudioManager& AudioManager::instance() {
    static AudioManager instance;
    return instance;
}

// Constructor - automatically loads saved settings
AudioManager::AudioManager() {
    loadSettings();
}

// Set master volume with automatic clamping and updates
void AudioManager::setMasterVolume(float volume) {
    // Clamp volume to valid range (0-100)
    m_masterVolume = std::max(0.0f, std::min(100.0f, volume));

    // Update all audio to reflect new master volume
    updateMusicVolume();
    updateSFXVolume();

    // Persist the change
    saveSettings();
}

// Set music volume with clamping and immediate application
void AudioManager::setMusicVolume(float volume) {
    m_musicVolume = std::max(0.0f, std::min(100.0f, volume));
    updateMusicVolume();
    saveSettings();
}

// Set SFX volume with clamping and immediate application
void AudioManager::setSFXVolume(float volume) {
    m_sfxVolume = std::max(0.0f, std::min(100.0f, volume));
    updateSFXVolume();
    saveSettings();
}

// Load music file and store in memory
bool AudioManager::loadMusic(const std::string& name, const std::string& filePath) {
    auto music = std::make_unique<sf::Music>();

    // Attempt to load from file
    if (!music->openFromFile(filePath)) {
        std::cerr << "Failed to load music: " << filePath << std::endl;
        return false;
    }

    // Store in map for later use
    m_music[name] = std::move(music);
    return true;
}

// Play specified music track
void AudioManager::playMusic(const std::string& name, bool loop) {
    auto it = m_music.find(name);
    if (it != m_music.end()) {
        // Stop current music if any
        if (m_currentMusic) {
            m_currentMusic->stop();
        }

        // Set up new music
        m_currentMusic = it->second.get();
        m_currentMusic->setLoop(loop);
        m_currentMusic->setVolume(getEffectiveVolume(m_musicVolume));
        m_currentMusic->play();
    }
}

// Stop currently playing music
void AudioManager::stopMusic() {
    if (m_currentMusic) {
        m_currentMusic->stop();
        m_currentMusic = nullptr;  // Clear reference
    }
}

// Pause current music (can be resumed later)
void AudioManager::pauseMusic() {
    if (m_currentMusic) {
        m_currentMusic->pause();
    }
}

// Resume paused music
void AudioManager::resumeMusic() {
    if (m_currentMusic) {
        m_currentMusic->play();
    }
}

// Load sound effect into memory
bool AudioManager::loadSound(const std::string& name, const std::string& filePath) {
    // Load audio data into buffer
    if (!m_soundBuffers[name].loadFromFile(filePath)) {
        std::cerr << "Failed to load sound: " << filePath << std::endl;
        return false;
    }

    // Create sound player and link to buffer
    m_sounds[name].setBuffer(m_soundBuffers[name]);
    return true;
}

// Play sound effect (can overlap with other sounds)
void AudioManager::playSound(const std::string& name) {
    auto it = m_sounds.find(name);
    if (it != m_sounds.end()) {
        // Apply current volume and play
        it->second.setVolume(getEffectiveVolume(m_sfxVolume));
        it->second.play();
    }
}

// Load all menu sounds
bool AudioManager::loadMenuSounds() {
    if (m_menuSoundsLoaded) {
        return true;
    }

    bool allLoaded = true;
    const std::string soundFile = "intro.wav";

    // Load sound for each menu screen
    allLoaded &= loadSound("menu_music", soundFile);
    allLoaded &= loadSound("loading_music", soundFile);
    allLoaded &= loadSound("settings_music", soundFile);
    allLoaded &= loadSound("help_music", soundFile);
    allLoaded &= loadSound("about_music", soundFile);
    allLoaded &= loadSound("button_hover", soundFile);
    allLoaded &= loadSound("button_click", soundFile);

    m_menuSoundsLoaded = allLoaded;
    return allLoaded;
}

// Menu sound playback functions
void AudioManager::playMenuSound() {
    if (m_menuSoundsEnabled && m_menuSoundsLoaded) {
        auto it = m_sounds.find("menu_music");
        if (it != m_sounds.end()) {
            it->second.setVolume(getEffectiveMenuVolume());
            it->second.play();
        }
    }
}

void AudioManager::playLoadingSound() {
    if (m_menuSoundsEnabled && m_menuSoundsLoaded) {
        auto it = m_sounds.find("loading_music");
        if (it != m_sounds.end()) {
            it->second.setVolume(getEffectiveMenuVolume());
            it->second.play();
        }
    }
}

void AudioManager::playSettingsSound() {
    if (m_menuSoundsEnabled && m_menuSoundsLoaded) {
        auto it = m_sounds.find("settings_music");
        if (it != m_sounds.end()) {
            it->second.setVolume(getEffectiveMenuVolume());
            it->second.play();
        }
    }
}

void AudioManager::playHelpSound() {
    if (m_menuSoundsEnabled && m_menuSoundsLoaded) {
        auto it = m_sounds.find("help_music");
        if (it != m_sounds.end()) {
            it->second.setVolume(getEffectiveMenuVolume());
            it->second.play();
        }
    }
}

void AudioManager::playAboutSound() {
    if (m_menuSoundsEnabled && m_menuSoundsLoaded) {
        auto it = m_sounds.find("about_music");
        if (it != m_sounds.end()) {
            it->second.setVolume(getEffectiveMenuVolume());
            it->second.play();
        }
    }
}

void AudioManager::playButtonHoverSound() {
    if (m_menuSoundsEnabled && m_menuSoundsLoaded) {
        auto it = m_sounds.find("button_hover");
        if (it != m_sounds.end()) {
            // Lower volume for hover effect
            it->second.setVolume(getEffectiveMenuVolume() * 0.5f);
            it->second.play();
        }
    }
}

void AudioManager::playButtonClickSound() {
    if (m_menuSoundsEnabled && m_menuSoundsLoaded) {
        auto it = m_sounds.find("button_click");
        if (it != m_sounds.end()) {
            it->second.setVolume(getEffectiveMenuVolume());
            it->second.play();
        }
    }
}

// Menu sound controls
void AudioManager::enableMenuSounds(bool enable) {
    m_menuSoundsEnabled = enable;
    saveSettings();
}

void AudioManager::setMenuSoundVolume(float volume) {
    m_menuSoundVolume = std::max(0.0f, std::min(100.0f, volume));
    saveSettings();
}

// System controls
void AudioManager::stopAllSounds() {
    // Stop all sound effects
    for (auto& sound : m_sounds) {
        sound.second.stop();
    }

    // Stop music
    stopMusic();
}

void AudioManager::resetAudioSystem() {
    stopAllSounds();

    // Reset all volume settings to defaults
    m_masterVolume = 100.0f;
    m_musicVolume = 100.0f;
    m_sfxVolume = 100.0f;
    m_menuSoundVolume = 100.0f;
    m_menuSoundsEnabled = true;

    saveSettings();
}

// Update volume of currently playing music
void AudioManager::updateMusicVolume() {
    if (m_currentMusic) {
        m_currentMusic->setVolume(getEffectiveVolume(m_musicVolume));
    }
}

// Update volume of all loaded sound effects
void AudioManager::updateSFXVolume() {
    for (auto& sound : m_sounds) {
        sound.second.setVolume(getEffectiveVolume(m_sfxVolume));
    }
}

// Calculate final volume considering both base and master volume
float AudioManager::getEffectiveVolume(float baseVolume) const {
    // Convert percentages to factors, multiply, then back to percentage
    return (baseVolume / 100.0f) * (m_masterVolume / 100.0f) * 100.0f;
}

// Calculate effective menu sound volume
float AudioManager::getEffectiveMenuVolume() const {
    // Menu sounds are affected by: menu volume -> SFX volume -> master volume
    return (m_menuSoundVolume / 100.0f) * (m_sfxVolume / 100.0f) * (m_masterVolume / 100.0f) * 100.0f;
}

// Save current volume settings to file
void AudioManager::saveSettings() {
    std::ofstream file("settings.txt");
    if (file.is_open()) {
        file << "master_volume=" << m_masterVolume << std::endl;
        file << "music_volume=" << m_musicVolume << std::endl;
        file << "sfx_volume=" << m_sfxVolume << std::endl;
        file << "menu_sounds_enabled=" << (m_menuSoundsEnabled ? 1 : 0) << std::endl;
        file << "menu_sound_volume=" << m_menuSoundVolume << std::endl;
        file.close();
    }
    else {
        std::cerr << "Failed to save audio settings" << std::endl;
    }
}

// Load volume settings from file
void AudioManager::loadSettings() {
    std::ifstream file("settings.txt");
    if (!file.is_open()) {
        return; // Use default values if file doesn't exist
    }

    std::string line;
    while (std::getline(file, line)) {
        // Parse key=value format
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string valueStr = line.substr(pos + 1);

            try {
                // Apply loaded values
                if (key == "master_volume") {
                    m_masterVolume = std::stof(valueStr);
                }
                else if (key == "music_volume") {
                    m_musicVolume = std::stof(valueStr);
                }
                else if (key == "sfx_volume") {
                    m_sfxVolume = std::stof(valueStr);
                }
                else if (key == "menu_sounds_enabled") {
                    m_menuSoundsEnabled = (std::stoi(valueStr) == 1);
                }
                else if (key == "menu_sound_volume") {
                    m_menuSoundVolume = std::stof(valueStr);
                }
            }
            catch (const std::exception& e) {
                std::cerr << "Error parsing setting " << key << ": " << e.what() << std::endl;
            }
        }
    }
    file.close();
}
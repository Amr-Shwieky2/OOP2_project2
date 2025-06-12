#include "AudioManager.h"
#include <fstream>
#include <iostream>

// Singleton instance getter
AudioManager& AudioManager::instance() {
    static AudioManager instance;  // Thread-safe in C++11+
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

// Save current volume settings to file
void AudioManager::saveSettings() {
    std::ofstream file("settings.txt");
    if (file.is_open()) {
        file << "master_volume=" << m_masterVolume << std::endl;
        file << "music_volume=" << m_musicVolume << std::endl;
        file << "sfx_volume=" << m_sfxVolume << std::endl;
    }
}

// Load volume settings from file (called in constructor)
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
            float value = std::stof(line.substr(pos + 1));

            // Apply loaded values
            if (key == "master_volume") {
                m_masterVolume = value;
            }
            else if (key == "music_volume") {
                m_musicVolume = value;
            }
            else if (key == "sfx_volume") {
                m_sfxVolume = value;
            }
        }
    }
}
#include "AudioManager.h"
#include <fstream>
#include <iostream>

AudioManager& AudioManager::instance() {
    static AudioManager instance;
    return instance;
}

AudioManager::AudioManager() {
    loadSettings();
}

void AudioManager::setMasterVolume(float volume) {
    m_masterVolume = std::max(0.0f, std::min(100.0f, volume));
    updateMusicVolume();
    updateSFXVolume();
    saveSettings();
}

void AudioManager::setMusicVolume(float volume) {
    m_musicVolume = std::max(0.0f, std::min(100.0f, volume));
    updateMusicVolume();
    saveSettings();
}

void AudioManager::setSFXVolume(float volume) {
    m_sfxVolume = std::max(0.0f, std::min(100.0f, volume));
    updateSFXVolume();
    saveSettings();
}

bool AudioManager::loadMusic(const std::string& name, const std::string& filePath) {
    auto music = std::make_unique<sf::Music>();
    if (!music->openFromFile(filePath)) {
        std::cerr << "Failed to load music: " << filePath << std::endl;
        return false;
    }

    m_music[name] = std::move(music);
    return true;
}

void AudioManager::playMusic(const std::string& name, bool loop) {
    auto it = m_music.find(name);
    if (it != m_music.end()) {
        if (m_currentMusic) {
            m_currentMusic->stop();
        }

        m_currentMusic = it->second.get();
        m_currentMusic->setLoop(loop);
        m_currentMusic->setVolume(getEffectiveVolume(m_musicVolume));
        m_currentMusic->play();
    }
}

void AudioManager::stopMusic() {
    if (m_currentMusic) {
        m_currentMusic->stop();
        m_currentMusic = nullptr;
    }
}

void AudioManager::pauseMusic() {
    if (m_currentMusic) {
        m_currentMusic->pause();
    }
}

void AudioManager::resumeMusic() {
    if (m_currentMusic) {
        m_currentMusic->play();
    }
}

bool AudioManager::loadSound(const std::string& name, const std::string& filePath) {
    if (!m_soundBuffers[name].loadFromFile(filePath)) {
        std::cerr << "Failed to load sound: " << filePath << std::endl;
        return false;
    }

    m_sounds[name].setBuffer(m_soundBuffers[name]);
    return true;
}

void AudioManager::playSound(const std::string& name) {
    auto it = m_sounds.find(name);
    if (it != m_sounds.end()) {
        it->second.setVolume(getEffectiveVolume(m_sfxVolume));
        it->second.play();
    }
}

void AudioManager::updateMusicVolume() {
    if (m_currentMusic) {
        m_currentMusic->setVolume(getEffectiveVolume(m_musicVolume));
    }
}

void AudioManager::updateSFXVolume() {
    for (auto& sound : m_sounds) {
        sound.second.setVolume(getEffectiveVolume(m_sfxVolume));
    }
}

float AudioManager::getEffectiveVolume(float baseVolume) const {
    return (baseVolume / 100.0f) * (m_masterVolume / 100.0f) * 100.0f;
}

void AudioManager::saveSettings() {
    std::ofstream file("settings.txt");
    if (file.is_open()) {
        file << "master_volume=" << m_masterVolume << std::endl;
        file << "music_volume=" << m_musicVolume << std::endl;
        file << "sfx_volume=" << m_sfxVolume << std::endl;
    }
}

void AudioManager::loadSettings() {
    std::ifstream file("settings.txt");
    if (!file.is_open()) {
        return; // Use default values
    }

    std::string line;
    while (std::getline(file, line)) {
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            float value = std::stof(line.substr(pos + 1));

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
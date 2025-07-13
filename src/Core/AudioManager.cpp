#include "../Core/AudioManager.h"
#include "../Core/SmartExceptions.h"
#include <iostream>
#include <algorithm>

using namespace SmartExceptions;

AudioManager& AudioManager::instance() {
    static AudioManager instance;
    return instance;
}

AudioManager::AudioManager()
    : m_masterVolume(100.0f), m_musicVolume(100.0f), m_sfxVolume(100.0f) {
    
    // Register audio recovery strategies
    RecoveryHandler::instance().registerRecoveryStrategy(Category::Audio, []() -> bool {
        // Try to reset audio system
        try {
            AudioManager::instance().resetAudioSystem();
            return true;
        } catch (...) {
            return false;
        }
    });
}

void AudioManager::setMasterVolume(float volume) {
    SAFE_EXECUTE(
        {
            if (volume < 0.0f || volume > 100.0f) {
                THROW_AUDIO_EXCEPTION(
                    "Invalid master volume: " + std::to_string(volume) + " (must be 0-100)",
                    Severity::Warning,
                    RecoveryStrategy::Fallback
                );
            }
            
            m_masterVolume = std::clamp(volume, 0.0f, 100.0f);
            updateMusicVolume();
            updateSFXVolume();
        },
        "setMasterVolume",
        "AudioManager",
        Category::Audio
    );
}

void AudioManager::setMusicVolume(float volume) {
    SAFE_EXECUTE(
        {
            if (volume < 0.0f || volume > 100.0f) {
                THROW_AUDIO_EXCEPTION(
                    "Invalid music volume: " + std::to_string(volume) + " (must be 0-100)",
                    Severity::Warning,
                    RecoveryStrategy::Fallback
                );
            }
            
            m_musicVolume = std::clamp(volume, 0.0f, 100.0f);
            updateMusicVolume();
        },
        "setMusicVolume",
        "AudioManager",
        Category::Audio
    );
}

void AudioManager::setSFXVolume(float volume) {
    SAFE_EXECUTE(
        {
            if (volume < 0.0f || volume > 100.0f) {
                THROW_AUDIO_EXCEPTION(
                    "Invalid SFX volume: " + std::to_string(volume) + " (must be 0-100)",
                    Severity::Warning,
                    RecoveryStrategy::Fallback
                );
            }
            
            m_sfxVolume = std::clamp(volume, 0.0f, 100.0f);
            updateSFXVolume();
        },
        "setSFXVolume",
        "AudioManager",
        Category::Audio
    );
}

float AudioManager::getMasterVolume() const {
    return m_masterVolume;
}

float AudioManager::getMusicVolume() const {
    return m_musicVolume;
}

float AudioManager::getSFXVolume() const {
    return m_sfxVolume;
}

bool AudioManager::loadMusic(const std::string& name, const std::string& filePath) {
    return SAFE_EXECUTE_WITH_RETURN(
        [&]() -> bool {
            if (name.empty()) {
                THROW_AUDIO_EXCEPTION(
                    "Music name cannot be empty",
                    Severity::Error,
                    RecoveryStrategy::Skip
                );
            }
            
            if (filePath.empty()) {
                THROW_AUDIO_EXCEPTION(
                    "Music file path cannot be empty for: " + name,
                    Severity::Error,
                    RecoveryStrategy::Skip
                );
            }
            
            auto music = std::make_unique<sf::Music>();
            if (!music->openFromFile(filePath)) {
                THROW_AUDIO_EXCEPTION(
                    "Failed to load music file: " + filePath,
                    Severity::Error,
                    RecoveryStrategy::Fallback
                );
            }
            
            m_music[name] = std::move(music);
            return true;
        },
        "loadMusic",
        "AudioManager",
        Category::Audio
    ).value_or(false);
}

void AudioManager::playMusic(const std::string& name, bool loop) {
    SAFE_EXECUTE(
        {
            if (name.empty()) {
                THROW_AUDIO_EXCEPTION(
                    "Music name cannot be empty",
                    Severity::Warning,
                    RecoveryStrategy::Skip
                );
            }
            
            auto it = m_music.find(name);
            if (it == m_music.end()) {
                THROW_AUDIO_EXCEPTION(
                    "Music not found: " + name,
                    Severity::Warning,
                    RecoveryStrategy::Skip
                );
            }
            
            if (m_currentMusic) {
                m_currentMusic->stop();
            }
            
            m_currentMusic = it->second.get();
            m_currentMusic->setLoop(loop);
            m_currentMusic->setVolume(getEffectiveVolume(m_musicVolume));
            m_currentMusic->play();
        },
        "playMusic",
        "AudioManager",
        Category::Audio
    );
}

void AudioManager::stopMusic() {
    SAFE_EXECUTE(
        {
            if (m_currentMusic) {
                m_currentMusic->stop();
            }
            m_currentMusic = nullptr;
        },
        "stopMusic",
        "AudioManager",
        Category::Audio
    );
}

void AudioManager::pauseMusic() {
    SAFE_EXECUTE(
        {
            if (m_currentMusic) {
                m_currentMusic->pause();
            } else {
                THROW_AUDIO_EXCEPTION(
                    "No music currently playing to pause",
                    Severity::Warning,
                    RecoveryStrategy::Skip
                );
            }
        },
        "pauseMusic",
        "AudioManager",
        Category::Audio
    );
}

void AudioManager::resumeMusic() {
    SAFE_EXECUTE(
        {
            if (m_currentMusic) {
                m_currentMusic->play();
            } else {
                THROW_AUDIO_EXCEPTION(
                    "No music currently available to resume",
                    Severity::Warning,
                    RecoveryStrategy::Skip
                );
            }
        },
        "resumeMusic",
        "AudioManager",
        Category::Audio
    );
}

bool AudioManager::loadSound(const std::string& name, const std::string& filePath) {
    return SAFE_EXECUTE_WITH_RETURN(
        [&]() -> bool {
            if (name.empty()) {
                THROW_AUDIO_EXCEPTION(
                    "Sound name cannot be empty",
                    Severity::Error,
                    RecoveryStrategy::Skip
                );
            }
            
            if (filePath.empty()) {
                THROW_AUDIO_EXCEPTION(
                    "Sound file path cannot be empty for: " + name,
                    Severity::Error,
                    RecoveryStrategy::Skip
                );
            }
            
            if (!m_soundBuffers[name].loadFromFile(filePath)) {
                THROW_AUDIO_EXCEPTION(
                    "Failed to load sound file: " + filePath,
                    Severity::Error,
                    RecoveryStrategy::Fallback
                );
            }
            
            m_sounds[name].setBuffer(m_soundBuffers[name]);
            return true;
        },
        "loadSound",
        "AudioManager",
        Category::Audio
    ).value_or(false);
}

void AudioManager::playSound(const std::string& name) {
    SAFE_EXECUTE(
        {
            if (name.empty()) {
                THROW_AUDIO_EXCEPTION(
                    "Sound name cannot be empty",
                    Severity::Warning,
                    RecoveryStrategy::Skip
                );
            }
            
            auto it = m_sounds.find(name);
            if (it == m_sounds.end()) {
                THROW_AUDIO_EXCEPTION(
                    "Sound not found: " + name,
                    Severity::Warning,
                    RecoveryStrategy::Skip
                );
            }
            
            it->second.setVolume(getEffectiveVolume(m_sfxVolume));
            it->second.play();
        },
        "playSound",
        "AudioManager",
        Category::Audio
    );
}

void AudioManager::updateMusicVolume() {
    SAFE_EXECUTE(
        {
            if (m_currentMusic) {
                m_currentMusic->setVolume(getEffectiveVolume(m_musicVolume));
            }
            
            for (auto& [name, music] : m_music) {
                if (music->getStatus() == sf::Music::Playing) {
                    music->setVolume(getEffectiveVolume(m_musicVolume));
                }
            }
        },
        "updateMusicVolume",
        "AudioManager",
        Category::Audio
    );
}

void AudioManager::updateSFXVolume() {
    SAFE_EXECUTE(
        {
            for (auto& [name, sound] : m_sounds) {
                sound.setVolume(getEffectiveVolume(m_sfxVolume));
            }
        },
        "updateSFXVolume",
        "AudioManager",
        Category::Audio
    );
}

void AudioManager::stopAllSounds() {
    SAFE_EXECUTE(
        {
            for (auto& [name, sound] : m_sounds) {
                sound.stop();
            }
            stopMusic();
        },
        "stopAllSounds",
        "AudioManager",
        Category::Audio
    );
}

void AudioManager::resetAudioSystem() {
    SAFE_EXECUTE(
        {
            setMasterVolume(100.0f);
            setMusicVolume(100.0f);
            setSFXVolume(100.0f);
        },
        "resetAudioSystem",
        "AudioManager",
        Category::Audio
    );
}

float AudioManager::getEffectiveVolume(float baseVolume) const {
    return (baseVolume / 100.0f) * (m_masterVolume / 100.0f) * 100.0f;
}

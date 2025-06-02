#include "AudioService.h"
#include "Logger.h"   // optional: to log errors/warnings
#include <stdexcept>
#include <iostream>

//-------------------------------------------
// 1) Singleton accessor
//-------------------------------------------
AudioService& AudioService::instance() {
    static AudioService instance;
    return instance;
}

//-------------------------------------------
// 2) Constructor & Destructor
//-------------------------------------------
AudioService::AudioService()
    : m_musicVolume(50.0f),
    m_effectsVolume(50.0f)
{
    // By default, set music volume to 50%
    m_music.setVolume(m_musicVolume);
}

AudioService::~AudioService() {
    // Nothing special needed: sf::Music will stop & destruct itself
    stopMusic();
}

//-------------------------------------------
// 3) loadSound
//-------------------------------------------
void AudioService::loadSound(const std::string& soundId, const std::string& filename) {
    std::lock_guard<std::mutex> lock(m_mutex);

    // If already loaded, skip or reload:
    if (m_soundBuffers.find(soundId) != m_soundBuffers.end()) {
        // Overwrite existing buffer
        m_soundBuffers.erase(soundId);
        m_sounds.erase(soundId);
    }

    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filename)) {
        // Failed to load sound file
        Logger::instance().logError("AudioService::loadSound(): failed to load \"" + filename + "\"");
        throw std::runtime_error("Failed to load sound file: " + filename);
    }

    // Insert the loaded buffer into our map
    m_soundBuffers.emplace(soundId, std::move(buffer));

    // Also create an sf::Sound that uses this buffer
    sf::Sound sound;
    sound.setBuffer(m_soundBuffers.at(soundId));
    sound.setVolume(m_effectsVolume);
    m_sounds.emplace(soundId, std::move(sound));

    Logger::instance().logInfo("AudioService: loaded sound \"" + soundId + "\" from " + filename);
}

//-------------------------------------------
// 4) playSound
//-------------------------------------------
void AudioService::playSound(const std::string& soundId) {
    std::lock_guard<std::mutex> lock(m_mutex);

    auto it = m_sounds.find(soundId);
    if (it == m_sounds.end()) {
        // Sound ID not found
        Logger::instance().logWarning("AudioService::playSound(): sound ID \"" + soundId + "\" not found");
        return;
    }

    // Before playing, ensure volume is set to current effects volume:
    it->second.setVolume(m_effectsVolume);
    it->second.play();
}

//-------------------------------------------
// 5) playMusic
//-------------------------------------------
void AudioService::playMusic(const std::string& musicFile, bool loop) {
    std::lock_guard<std::mutex> lock(m_mutex);

    // If music was already playing, stop it first:
    if (m_music.getStatus() == sf::Music::Playing) {
        m_music.stop();
    }

    if (!m_music.openFromFile(musicFile)) {
        Logger::instance().logError("AudioService::playMusic(): failed to load \"" + musicFile + "\"");
        throw std::runtime_error("Failed to load music file: " + musicFile);
    }

    m_music.setLoop(loop);
    m_music.setVolume(m_musicVolume);
    m_music.play();

    Logger::instance().logInfo("AudioService: playing music \"" + musicFile + "\" (loop=" + (loop ? "true" : "false") + ")");
}

//-------------------------------------------
// 6) stopMusic
//-------------------------------------------
void AudioService::stopMusic() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_music.getStatus() == sf::Music::Playing) {
        m_music.stop();
        Logger::instance().logInfo("AudioService: stopped music");
    }
}

//-------------------------------------------
// 7) setMusicVolume
//-------------------------------------------
void AudioService::setMusicVolume(float volume) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_musicVolume = volume;
    m_music.setVolume(m_musicVolume);
    Logger::instance().logInfo("AudioService: music volume set to " + std::to_string(volume));
}

//-------------------------------------------
// 8) setEffectsVolume
//-------------------------------------------
void AudioService::setEffectsVolume(float volume) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_effectsVolume = volume;
    // Update all existing sf::Sound objects
    for (auto& pair : m_sounds) {
        pair.second.setVolume(m_effectsVolume);
    }
    Logger::instance().logInfo("AudioService: effects volume set to " + std::to_string(volume));
}

//-------------------------------------------
// 9) hasSound (helper, not strictly needed)
//-------------------------------------------
bool AudioService::hasSound(const std::string& soundId) const {
    return (m_soundBuffers.find(soundId) != m_soundBuffers.end());
}

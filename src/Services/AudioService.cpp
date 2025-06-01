#include "AudioService.h"
#include <iostream>
#include <stdexcept>

AudioService::AudioService() {
    // Preload default sounds here if desired
    // Example:
    // loadSound("click", "click.wav");
}

void AudioService::loadSound(const std::string& soundId, const std::string& filename) {
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filename)) {
        throw std::runtime_error("Failed to load sound file: " + filename);
    }

    m_soundBuffers[soundId] = buffer;
    sf::Sound sound;
    sound.setBuffer(m_soundBuffers[soundId]);
    m_sounds[soundId] = sound;
}

void AudioService::playSound(const std::string& soundId) {
    auto it = m_sounds.find(soundId);
    if (it != m_sounds.end()) {
        it->second.setVolume(m_effectsVolume);
        it->second.play();
    }
    else {
        std::cerr << "Sound ID not found: " << soundId << std::endl;
    }
}

void AudioService::playMusic(const std::string& musicFile, bool loop) {
    if (!m_music.openFromFile(musicFile)) {
        throw std::runtime_error("Failed to load music file: " + musicFile);
    }

    m_music.setLoop(loop);
    m_music.setVolume(m_musicVolume);
    m_music.play();
}

void AudioService::stopMusic() {
    m_music.stop();
}

void AudioService::setMusicVolume(float volume) {
    m_musicVolume = volume;
    m_music.setVolume(volume);
}

void AudioService::setEffectsVolume(float volume) {
    m_effectsVolume = volume;
    for (auto& pair : m_sounds) {
        pair.second.setVolume(volume);
    }
}

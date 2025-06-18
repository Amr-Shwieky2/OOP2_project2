#pragma once

#include <SFML/Audio.hpp>
#include <string>
#include <unordered_map>
#include <memory>

// Handles sound effects and background music
class AudioService {
public:
    AudioService();
    ~AudioService() = default;

    // Load and play a named sound effect
    void playSound(const std::string& soundId);

    // Load and play background music
    void playMusic(const std::string& musicFile, bool loop = true);

    // Stop music playback
    void stopMusic();

    // Volume controls
    void setMusicVolume(float volume);   // 0 - 100
    void setEffectsVolume(float volume); // 0 - 100

private:
    void loadSound(const std::string& soundId, const std::string& filename);

    std::unordered_map<std::string, sf::SoundBuffer> m_soundBuffers;
    std::unordered_map<std::string, sf::Sound> m_sounds;

    sf::Music m_music;
    float m_musicVolume = 100.0f;
    float m_effectsVolume = 100.0f;
};

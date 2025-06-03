#pragma once

#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>
#include <memory>

class AudioManager {
public:
    static AudioManager& instance();

    // Volume control (0.0f to 100.0f)
    void setMasterVolume(float volume);
    void setMusicVolume(float volume);
    void setSFXVolume(float volume);

    float getMasterVolume() const { return m_masterVolume; }
    float getMusicVolume() const { return m_musicVolume; }
    float getSFXVolume() const { return m_sfxVolume; }

    // Music control
    bool loadMusic(const std::string& name, const std::string& filePath);
    void playMusic(const std::string& name, bool loop = true);
    void stopMusic();
    void pauseMusic();
    void resumeMusic();

    // Sound effects
    bool loadSound(const std::string& name, const std::string& filePath);
    void playSound(const std::string& name);

    // Settings persistence
    void saveSettings();
    void loadSettings();

private:
    AudioManager();

    float m_masterVolume = 100.0f;
    float m_musicVolume = 100.0f;
    float m_sfxVolume = 100.0f;

    std::unordered_map<std::string, std::unique_ptr<sf::Music>> m_music;
    std::unordered_map<std::string, sf::SoundBuffer> m_soundBuffers;
    std::unordered_map<std::string, sf::Sound> m_sounds;

    sf::Music* m_currentMusic = nullptr;

    void updateMusicVolume();
    void updateSFXVolume();
    float getEffectiveVolume(float baseVolume) const;
};

#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include <SFML/Audio.hpp>

class MenuSoundManager {
public:
    static MenuSoundManager& instance();

    MenuSoundManager();

    bool loadSounds(const std::string& sharedFilePath = "intro.wav");

    void playMenuMusic(const std::string& key);
    void playButtonHoverSound();
    void playButtonClickSound();

    void setVolume(float volume);
    void enable(bool enable);
    bool isEnabled() const;
    float getVolume() const;

    void stop();
    void reset();

private:
    std::unordered_map<std::string, std::unique_ptr<sf::Music>> m_music;
    std::unordered_map<std::string, sf::SoundBuffer> m_soundBuffers;
    std::unordered_map<std::string, sf::Sound> m_sounds;

    sf::Music* m_current = nullptr;
    float m_volume = 100.0f;
    bool m_enabled = true;
};

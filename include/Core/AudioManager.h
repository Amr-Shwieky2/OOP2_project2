#pragma once
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>
#include <memory>

/**
 * @brief Singleton Audio Manager for centralized sound and music control
 *
 * Manages all audio resources in the application including:
 * - Background music with looping support
 * - Sound effects with multiple simultaneous playback
 * - Volume controls (master, music, SFX)
 * - Settings persistence to file
 */
class AudioManager {
public:
    // Singleton access
    static AudioManager& instance();

    // Volume control (0.0f to 100.0f range)
    void setMasterVolume(float volume);  // Controls overall audio level
    void setMusicVolume(float volume);   // Controls background music level
    void setSFXVolume(float volume);     // Controls sound effects level

    // Volume getters - inline for performance
    float getMasterVolume() const { return m_masterVolume; }
    float getMusicVolume() const { return m_musicVolume; }
    float getSFXVolume() const { return m_sfxVolume; }

    // Music control - typically one music track at a time
    bool loadMusic(const std::string& name, const std::string& filePath);
    void playMusic(const std::string& name, bool loop = true);
    void stopMusic();
    void pauseMusic();
    void resumeMusic();

    // Sound effects - can play multiple sounds simultaneously
    bool loadSound(const std::string& name, const std::string& filePath);
    void playSound(const std::string& name);

    // Settings persistence
    void saveSettings();  // Save volume settings to file
    void loadSettings();  // Load volume settings from file

private:
    // Private constructor for Singleton pattern
    AudioManager();

    // Volume levels (0-100 range)
    float m_masterVolume = 100.0f;
    float m_musicVolume = 100.0f;
    float m_sfxVolume = 100.0f;

    // Music storage - using unique_ptr for proper memory management
    std::unordered_map<std::string, std::unique_ptr<sf::Music>> m_music;

    // Sound effects storage
    std::unordered_map<std::string, sf::SoundBuffer> m_soundBuffers;  // Audio data
    std::unordered_map<std::string, sf::Sound> m_sounds;              // Sound players

    // Currently playing music track
    sf::Music* m_currentMusic = nullptr;

    // Helper methods for volume updates
    void updateMusicVolume();    // Apply volume changes to current music
    void updateSFXVolume();      // Apply volume changes to all sounds
    float getEffectiveVolume(float baseVolume) const;  // Calculate final volume
};
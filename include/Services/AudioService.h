#pragma once

#include <SFML/Audio.hpp>
#include <string>
#include <unordered_map>
#include <mutex>

/**
 * AudioService is a singleton that allows:
 *  • Loading sound effects (sf::SoundBuffer + sf::Sound)
 *  • Playing short sound effects by ID
 *  • Playing/looping background music (sf::Music)
 *  • Adjusting volumes for music and sound effects
 */
class AudioService {
public:
    // Get the sole instance of AudioService
    static AudioService& instance();

    ~AudioService();


    // Delete copy/move so no duplicates
    AudioService(const AudioService&) = delete;
    AudioService& operator=(const AudioService&) = delete;
    AudioService(AudioService&&) = delete;
    AudioService& operator=(AudioService&&) = delete;

    // Load a sound effect from file, associate it with a string key (e.g. "click")
    // Throws std::runtime_error if loading fails.
    void loadSound(const std::string& soundId, const std::string& filename);

    // Play a previously loaded sound effect by its ID. If ID not found, does nothing (or logs warning).
    void playSound(const std::string& soundId);

    // Start background music (streaming). 
    // If loop == true, music will loop until stopped.
    // Throws std::runtime_error if loading fails.
    void playMusic(const std::string& musicFile, bool loop = true);

    // Stop the currently playing music (if any)
    void stopMusic();

    // Adjust the volume [0..100] for background music
    void setMusicVolume(float volume);

    // Adjust the volume [0..100] for all sound effects
    void setEffectsVolume(float volume);

private:
    AudioService();   // private ctor

    // Internal helper to check if a soundId exists
    bool hasSound(const std::string& soundId) const;

    // Members:
    std::mutex                                   m_mutex;       // guards the maps & sf::Music
    std::unordered_map<std::string, sf::SoundBuffer> m_soundBuffers;
    std::unordered_map<std::string, sf::Sound>       m_sounds;
    sf::Music                                    m_music;       // only one music stream at a time
    float                                        m_musicVolume;   // [0..100]
    float                                        m_effectsVolume; // [0..100]
};

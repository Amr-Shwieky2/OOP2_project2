#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

// Forward declarations
class Slider;

/**
 * Volume Control Panel - handles only volume-related UI and logic
 * Single Responsibility: Volume management
 */
class VolumeControlPanel {
public:
    explicit VolumeControlPanel(const sf::Font& font);
    ~VolumeControlPanel() ;

    // Core functionality
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    bool handleMouseEvent(const sf::Event& event);

    // Volume management
    void refreshFromAudioManager();
    void saveSettings();

    // State queries
    bool hasChanged() const { return m_hasChanged; }
    void markSaved() { m_hasChanged = false; }

    // Layout configuration
    void setPosition(sf::Vector2f position) { m_position = position; }
    void setSpacing(float spacing) { m_spacing = spacing; }

private:
    // Single volume slider component
    struct VolumeSlider {
        sf::Text label;
        std::unique_ptr<Slider> slider;
        sf::Text value;
        std::string type; // "master", "music", "sfx"

        VolumeSlider(const std::string& labelText, const std::string& sliderType,
            const sf::Font& font, sf::Vector2f position);

        void updateValueText();
        void setVolume(float volume);
        float getVolume() const;
    };

    // Volume sliders
    std::unique_ptr<VolumeSlider> m_masterVolume;
    std::unique_ptr<VolumeSlider> m_musicVolume;
    std::unique_ptr<VolumeSlider> m_sfxVolume;

    // Panel properties
    sf::Vector2f m_position;
    float m_spacing = 90.0f;
    bool m_hasChanged = false;

    // Visual styling
    sf::Color m_labelColor = sf::Color(160, 82, 45);  // Saddle Brown
    sf::Color m_valueColor = sf::Color(139, 69, 19);  // Dark Brown

    // Setup methods - each with single purpose
    void setupMasterVolumeSlider(const sf::Font& font);
    void setupMusicVolumeSlider(const sf::Font& font);
    void setupSFXVolumeSlider(const sf::Font& font);

    // Event handling
    void onVolumeChanged(const std::string& type, float value);

    // Helper methods
    sf::Vector2f calculateSliderPosition(int index) const;
    void updateAllValueTexts();
    void applyVolumeToAudioManager(const std::string& type, float value);
};
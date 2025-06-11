#pragma once
#include "../Core/IScreen.h"
#include "../Core/UITheme.h"
#include "../UI/SettingsUIFactory.h"
#include "../UI/Dropdown.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <unordered_map>
#include <functional>
#include <string>

// Strategy Pattern for Settings Storage
class ISettingsStorage {
public:
    virtual ~ISettingsStorage() = default;
    virtual void saveSettings(const std::unordered_map<std::string, float>& settings) = 0;
    virtual std::unordered_map<std::string, float> loadSettings() = 0;
    virtual bool isValid() const = 0;
};

// File-based storage
class FileSettingsStorage : public ISettingsStorage {
public:
    FileSettingsStorage(const std::string& filename) : m_filename(filename) {}

    void saveSettings(const std::unordered_map<std::string, float>& settings) override;
    std::unordered_map<std::string, float> loadSettings() override;
    bool isValid() const override;

private:
    std::string m_filename;
};

// Template Settings Manager
template<typename SettingType>
class SettingsManager {
public:
    SettingsManager(std::unique_ptr<ISettingsStorage> storage)
        : m_storage(std::move(storage)) {
    }

    void setSetting(const std::string& key, SettingType value) {
        m_settings[key] = value;
    }

    SettingType getSetting(const std::string& key, SettingType defaultValue = SettingType{}) const {
        auto it = m_settings.find(key);
        return (it != m_settings.end()) ? it->second : defaultValue;
    }

    void save() {
        if (m_storage && m_storage->isValid()) {
            std::unordered_map<std::string, float> floatSettings;
            for (const auto& [key, value] : m_settings) {
                floatSettings[key] = static_cast<float>(value);
            }
            m_storage->saveSettings(floatSettings);
        }
    }

    void load() {
        if (m_storage && m_storage->isValid()) {
            auto floatSettings = m_storage->loadSettings();
            for (const auto& [key, value] : floatSettings) {
                m_settings[key] = static_cast<SettingType>(value);
            }
        }
    }

    size_t getSettingsCount() const { return m_settings.size(); }
    void clear() { m_settings.clear(); }

private:
    std::unordered_map<std::string, SettingType> m_settings;
    std::unique_ptr<ISettingsStorage> m_storage;
};

// Enhanced SettingsScreen with Crash Protection
class SettingsScreen : public IScreen {
public:
    SettingsScreen();
    ~SettingsScreen() = default;

    // IScreen interface
    void handleEvents(sf::RenderWindow& window) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    // Settings Management
    std::unique_ptr<SettingsManager<float>> m_settingsManager;

    // Resource Management
    sf::Font m_font;
    sf::Texture m_backgroundTexture;
    sf::Sprite m_backgroundSprite;

    // UI Theme and Layout
    UITheme::Colors m_colors;
    UITheme::Layout m_layout;

    // UI Components
    sf::Text m_titleText;
    sf::Text m_backText;

    SettingsUIFactory::VolumeControls m_masterVolume;
    SettingsUIFactory::VolumeControls m_musicVolume;
    SettingsUIFactory::VolumeControls m_sfxVolume;

    sf::Text m_languageLabel;
    std::unique_ptr<Dropdown> m_languageDropdown;

    // Animation and State
    float m_animationTime = 0.0f;
    bool m_settingsChanged = false;
    sf::Clock m_autoSaveClock;

    // Methods
    void initializeSettingsManager();
    void loadResources();
    void loadBackground();
    void setupTheme();
    void setupUI();
    void setupVolumeControls();
    void setupLanguageControl();
    void updateVolumeTexts();
    void updateLanguageTexts();

    // NEW: Language preference loading
    void loadLanguagePreference();

    // NEW: Fallback UI setup for error cases
    void setupFallbackUI();

    // NEW: Emergency fallback for critical errors
    void setupEmergencyFallback();

    // Event handlers
    void onMasterVolumeChanged(float volume);
    void onMusicVolumeChanged(float volume);
    void onSFXVolumeChanged(float volume);
    void onLanguageChanged(int index, int value);

    // Utility
    void autoSaveSettings();
    void resetToDefaults();
};
#pragma once
#include "../Core/IScreen.h"
#include "../UI/SettingsUIFactory.h"
#include "../Core/UITheme.h"
#include <SFML/Graphics.hpp>

class SettingsScreen : public IScreen {
public:
    SettingsScreen();

    void handleEvents(sf::RenderWindow& window) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    // Core components
    sf::Font m_font;
    sf::Texture m_backgroundTexture;
    sf::Sprite m_backgroundSprite;

    // Theme and layout
    UITheme::Colors m_colors;
    UITheme::Layout m_layout;

    // UI Elements
    sf::Text m_titleText;
    sf::Text m_backText;

    SettingsUIFactory::VolumeControls m_masterVolume;
    SettingsUIFactory::VolumeControls m_musicVolume;
    SettingsUIFactory::VolumeControls m_sfxVolume;

    sf::Text m_languageLabel;
    std::unique_ptr<Dropdown> m_languageDropdown;

    // Methods
    void loadResources();
    void loadBackground();
    void setupTheme();
    void setupUI();
    void setupVolumeControls();
    void setupLanguageControl();
    void updateVolumeTexts();
    void updateLanguageTexts();

    // Event handlers
    void onMasterVolumeChanged(float volume);
    void onMusicVolumeChanged(float volume);
    void onSFXVolumeChanged(float volume);
    void onLanguageChanged(int index, int value);
};

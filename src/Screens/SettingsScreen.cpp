#include "../../include/Screens/SettingsScreen.h"
#include "AppContext.h"
#include "ScreenTypes.h"
#include "../Core/AudioManager.h"
#include "../Core/LanguageManager.h"
#include <iostream>
#include <fstream>

SettingsScreen::SettingsScreen() {
    loadResources();
    setupTheme();
    setupUI();
    updateLanguageTexts();
}

void SettingsScreen::loadResources() {
    // Load font with fallbacks
    if (!m_font.loadFromFile("arial.ttf")) {
          std::cout << "Using default font" << std::endl;
    }

    loadBackground();
}

void SettingsScreen::loadBackground() {
    try {
        if (m_backgroundTexture.loadFromFile("SettingsScreen.png")) {
            m_backgroundSprite.setTexture(m_backgroundTexture);

            // Scale to fit window
            sf::Vector2u textureSize = m_backgroundTexture.getSize();
            sf::Vector2f targetSize(1400.0f, 800.0f);
            float scaleX = targetSize.x / textureSize.x;
            float scaleY = targetSize.y / textureSize.y;
            m_backgroundSprite.setScale(scaleX, scaleY);

        }
        else {
            std::cout << "Background image not found: SettingsScreen.png" << std::endl;
        }
    }
    catch (...) {
        std::cout << "Failed to load background" << std::endl;
    }
}

void SettingsScreen::setupTheme() {
    m_colors = UITheme::getDesertTheme();
    m_layout = UITheme::getDefaultLayout();
}

void SettingsScreen::setupUI() {
    // Title
    m_titleText = SettingsUIFactory::createTitle(
        "Settings",
        sf::Vector2f(550, 50),
        m_font,
        m_colors
    );

    // Back instruction
    m_backText = SettingsUIFactory::createLabel(
        "Press ESC to go back",
        sf::Vector2f(500, 650),
        m_font,
        m_colors,
        20
    );
    m_backText.setFillColor(sf::Color(180, 180, 180));
    m_backText.setStyle(sf::Text::Italic);

    setupVolumeControls();
    setupLanguageControl();
}

void SettingsScreen::setupVolumeControls() {
    auto& audioManager = AudioManager::instance();

    // Master Volume
    m_masterVolume = SettingsUIFactory::createVolumeControl(
        "Master Volume:",
        sf::Vector2f(m_layout.labelX, m_layout.startY),
        m_font,
        m_colors,
        m_layout,
        audioManager.getMasterVolume()
    );
    m_masterVolume.slider->setOnValueChanged([this](float value) {
        onMasterVolumeChanged(value);
        });

    // Music Volume
    m_musicVolume = SettingsUIFactory::createVolumeControl(
        "Music Volume:",
        sf::Vector2f(m_layout.labelX, m_layout.startY + m_layout.spacing),
        m_font,
        m_colors,
        m_layout,
        audioManager.getMusicVolume()
    );
    m_musicVolume.slider->setOnValueChanged([this](float value) {
        onMusicVolumeChanged(value);
        });

    // SFX Volume
    m_sfxVolume = SettingsUIFactory::createVolumeControl(
        "SFX Volume:",
        sf::Vector2f(m_layout.labelX, m_layout.startY + m_layout.spacing * 2),
        m_font,
        m_colors,
        m_layout,
        audioManager.getSFXVolume()
    );
    m_sfxVolume.slider->setOnValueChanged([this](float value) {
        onSFXVolumeChanged(value);
        });
}

void SettingsScreen::setupLanguageControl() {
    // Language label
    m_languageLabel = SettingsUIFactory::createLabel(
        "Language:",
        sf::Vector2f(m_layout.labelX, m_layout.startY + m_layout.spacing * 3),
        m_font,
        m_colors
    );

    // Language dropdown
    m_languageDropdown = SettingsUIFactory::createLanguageDropdown(
        sf::Vector2f(m_layout.labelX, m_layout.startY + m_layout.spacing * 3),
        m_font,
        m_colors,
        m_layout
    );

    m_languageDropdown->setOnSelectionChanged([this](int index, int value) {
        onLanguageChanged(index, value);
        });
}

void SettingsScreen::handleEvents(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                AppContext::instance().screenManager().changeScreen(ScreenType::MENU);
            }
        }

        // Handle mouse events
        if (event.type == sf::Event::MouseMoved) {
            sf::Vector2f mousePos(event.mouseMove.x, event.mouseMove.y);
            m_masterVolume.slider->handleMouseMove(mousePos);
            m_musicVolume.slider->handleMouseMove(mousePos);
            m_sfxVolume.slider->handleMouseMove(mousePos);
            m_languageDropdown->handleMouseMove(mousePos);
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

                m_masterVolume.slider->handleMousePressed(mousePos) ||
                    m_musicVolume.slider->handleMousePressed(mousePos) ||
                    m_sfxVolume.slider->handleMousePressed(mousePos) ||
                    m_languageDropdown->handleMousePressed(mousePos);
            }
        }

        if (event.type == sf::Event::MouseButtonReleased) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                m_masterVolume.slider->handleMouseReleased();
                m_musicVolume.slider->handleMouseReleased();
                m_sfxVolume.slider->handleMouseReleased();
            }
        }
    }
}

void SettingsScreen::update(float deltaTime) {
    m_masterVolume.slider->update(deltaTime);
    m_musicVolume.slider->update(deltaTime);
    m_sfxVolume.slider->update(deltaTime);
    m_languageDropdown->update(deltaTime);
}

void SettingsScreen::render(sf::RenderWindow& window) {
    // Draw background
    if (m_backgroundTexture.getSize().x > 0) {
        window.draw(m_backgroundSprite);
    }

    // Draw UI elements
    window.draw(m_titleText);

    // Volume controls
    window.draw(m_masterVolume.label);
    m_masterVolume.slider->render(window);
    window.draw(m_masterVolume.value);

    window.draw(m_musicVolume.label);
    m_musicVolume.slider->render(window);
    window.draw(m_musicVolume.value);

    window.draw(m_sfxVolume.label);
    m_sfxVolume.slider->render(window);
    window.draw(m_sfxVolume.value);

    // Language control
    window.draw(m_languageLabel);
    m_languageDropdown->render(window);

    window.draw(m_backText);
}

void SettingsScreen::updateVolumeTexts() {
    m_masterVolume.value.setString(std::to_string(static_cast<int>(m_masterVolume.slider->getValue())) + "%");
    m_musicVolume.value.setString(std::to_string(static_cast<int>(m_musicVolume.slider->getValue())) + "%");
    m_sfxVolume.value.setString(std::to_string(static_cast<int>(m_sfxVolume.slider->getValue())) + "%");
}

void SettingsScreen::updateLanguageTexts() {
    auto& langManager = LanguageManager::instance();

    m_titleText.setString(langManager.getText("settings_title"));
    m_masterVolume.label.setString(langManager.getText("settings_volume") + " (Master):");
    m_musicVolume.label.setString(langManager.getText("settings_volume") + " (Music):");
    m_sfxVolume.label.setString(langManager.getText("settings_volume") + " (SFX):");
    m_languageLabel.setString(langManager.getText("settings_language") + ":");
    m_backText.setString(langManager.getText("settings_back"));
}

void SettingsScreen::onMasterVolumeChanged(float volume) {
    AudioManager::instance().setMasterVolume(volume);
    updateVolumeTexts();
}

void SettingsScreen::onMusicVolumeChanged(float volume) {
    AudioManager::instance().setMusicVolume(volume);
    updateVolumeTexts();
}

void SettingsScreen::onSFXVolumeChanged(float volume) {
    AudioManager::instance().setSFXVolume(volume);
    updateVolumeTexts();
}

void SettingsScreen::onLanguageChanged(int index, int value) {
    Language newLang = static_cast<Language>(value);
    LanguageManager::instance().setLanguage(newLang);
    updateLanguageTexts();

    // Save language preference
    std::ofstream file("language.txt");
    if (file.is_open()) {
        file << "language=" << value << std::endl;
    }
}
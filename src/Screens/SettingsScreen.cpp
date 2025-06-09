// src/Screens/SettingsScreen.cpp - FIXED VERSION
#include "../../include/Screens/SettingsScreen.h"
#include "AppContext.h"
#include "ScreenTypes.h"
#include "../Core/AudioManager.h"
#include "../Core/LanguageManager.h"
#include "../Core/MultiFontManager.h"
#include <iostream>
#include <fstream>

SettingsScreen::SettingsScreen() {
    // Initialize components in correct order
    initializeSettingsManager();
    loadLanguagePreference();
    MultiFontManager::instance().loadLanguageFonts(); // Load fonts
    loadResources();
    setupTheme();
    setupUI();
    updateLanguageTexts();
}

void SettingsScreen::initializeSettingsManager() {
    try {
        auto storage = std::make_unique<FileSettingsStorage>("settings.dat");
        m_settingsManager = std::make_unique<SettingsManager<float>>(std::move(storage));
        m_settingsManager->load();
        std::cout << "Settings manager initialized successfully" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Failed to initialize settings manager: " << e.what() << std::endl;
    }
}

void SettingsScreen::loadLanguagePreference() {
    try {
        std::ifstream file("language.txt");
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                if (line.length() == 0) continue;

                size_t pos = line.find('=');
                if (pos != std::string::npos && pos > 0 && pos < line.length() - 1) {
                    std::string key = line.substr(0, pos);
                    std::string valueStr = line.substr(pos + 1);

                    if (key == "language" && !valueStr.empty()) {
                        try {
                            int value = std::stoi(valueStr);
                            if (value >= 0 && value <= 2) {
                                Language savedLang = static_cast<Language>(value);
                                LanguageManager::instance().setLanguage(savedLang);
                                std::cout << "Loaded saved language: " << value << std::endl;
                                break;
                            }
                        }
                        catch (const std::exception& e) {
                            std::cout << "Error parsing language value: " << e.what() << std::endl;
                        }
                    }
                }
            }
            file.close();
        }
        else {
            LanguageManager::instance().setLanguage(Language::ENGLISH);
            std::cout << "Using default language: English" << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error loading language preference: " << e.what() << std::endl;
        LanguageManager::instance().setLanguage(Language::ENGLISH);
    }
}

void SettingsScreen::loadResources() {
    try {
        // Get the font for current language from MultiFontManager
        m_font = MultiFontManager::instance().getCurrentFont();
        std::cout << "Using font from MultiFontManager for current language" << std::endl;
        loadBackground();
    }
    catch (const std::exception& e) {
        std::cout << "Error loading resources: " << e.what() << std::endl;
    }
}

void SettingsScreen::loadBackground() {
    try {
        if (m_backgroundTexture.loadFromFile("SettingsScreen.png")) {
            m_backgroundSprite.setTexture(m_backgroundTexture);
            sf::Vector2u textureSize = m_backgroundTexture.getSize();
            if (textureSize.x > 0 && textureSize.y > 0) {
                sf::Vector2f targetSize(1400.0f, 800.0f);
                float scaleX = targetSize.x / textureSize.x;
                float scaleY = targetSize.y / textureSize.y;
                m_backgroundSprite.setScale(scaleX, scaleY);
            }
        }
        else {
            std::cout << "Background image not found: SettingsScreen.png" << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cout << "Failed to load background: " << e.what() << std::endl;
    }
}

void SettingsScreen::setupTheme() {
    m_colors = UITheme::getDesertTheme();
    m_layout = UITheme::getDefaultLayout();
}

void SettingsScreen::setupUI() {
    auto& lang = LanguageManager::instance();

    // Title
    m_titleText = SettingsUIFactory::createTitle(
        lang.getText("settings_title"),
        sf::Vector2f(550, 50),
        m_font,
        m_colors
    );

    // Back text
    m_backText.setFont(m_font);
    m_backText.setString(lang.getText("settings_back"));
    m_backText.setCharacterSize(20);
    m_backText.setPosition(500, 650);
    m_backText.setFillColor(sf::Color(180, 180, 180));
    m_backText.setStyle(sf::Text::Italic);

    setupVolumeControls();
    setupLanguageControl();
}

void SettingsScreen::setupVolumeControls() {
    try {
        auto& audioManager = AudioManager::instance();
        auto& lang = LanguageManager::instance();

        std::string volumeText = lang.getText("settings_volume");
        if (volumeText.empty()) {
            volumeText = "Volume";
        }

        // Master Volume
        m_masterVolume = SettingsUIFactory::createVolumeControl(
            sf::String(volumeText + " (Master):"),
            sf::Vector2f(m_layout.labelX, m_layout.startY),
            m_font,
            m_colors,
            m_layout,
            audioManager.getMasterVolume()
        );

        if (m_masterVolume.slider) {
            m_masterVolume.slider->setOnValueChanged([this](float value) {
                try {
                    onMasterVolumeChanged(value);
                }
                catch (...) {
                    std::cout << "Error in master volume callback" << std::endl;
                }
                });
        }

        // Music Volume
        m_musicVolume = SettingsUIFactory::createVolumeControl(
            sf::String(volumeText + " (Music):"),
            sf::Vector2f(m_layout.labelX, m_layout.startY + m_layout.spacing),
            m_font,
            m_colors,
            m_layout,
            audioManager.getMusicVolume()
        );

        if (m_musicVolume.slider) {
            m_musicVolume.slider->setOnValueChanged([this](float value) {
                try {
                    onMusicVolumeChanged(value);
                }
                catch (...) {
                    std::cout << "Error in music volume callback" << std::endl;
                }
                });
        }

        // SFX Volume
        m_sfxVolume = SettingsUIFactory::createVolumeControl(
            sf::String(volumeText + " (SFX):"),
            sf::Vector2f(m_layout.labelX, m_layout.startY + m_layout.spacing * 2),
            m_font,
            m_colors,
            m_layout,
            audioManager.getSFXVolume()
        );

        if (m_sfxVolume.slider) {
            m_sfxVolume.slider->setOnValueChanged([this](float value) {
                try {
                    onSFXVolumeChanged(value);
                }
                catch (...) {
                    std::cout << "Error in SFX volume callback" << std::endl;
                }
                });
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error setting up volume controls: " << e.what() << std::endl;
    }
}

void SettingsScreen::setupLanguageControl() {
    auto& lang = LanguageManager::instance();

    // Language label
    m_languageLabel = SettingsUIFactory::createLabel(
        lang.getText("settings_language") + ":",
        sf::Vector2f(m_layout.labelX, m_layout.startY + m_layout.spacing * 3),
        m_font,
        m_colors
    );

    // Language dropdown
    m_languageDropdown = SettingsUIFactory::createLanguageDropdown(
        sf::Vector2f(m_layout.controlX, m_layout.startY + m_layout.spacing * 3),
        m_font,
        m_colors,
        m_layout
    );

    if (m_languageDropdown) {
        m_languageDropdown->setOnSelectionChanged([this](int index, int value) {
            onLanguageChanged(index, value);
            });
    }
}

void SettingsScreen::handleEvents(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        try {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    AppContext::instance().screenManager().changeScreen(ScreenType::MENU);
                }
            }

            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2f mousePos(static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y));

                if (m_masterVolume.slider) m_masterVolume.slider->handleMouseMove(mousePos);
                if (m_musicVolume.slider) m_musicVolume.slider->handleMouseMove(mousePos);
                if (m_sfxVolume.slider) m_sfxVolume.slider->handleMouseMove(mousePos);
                if (m_languageDropdown) m_languageDropdown->handleMouseMove(mousePos);
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

                    bool handled = false;
                    if (m_masterVolume.slider) handled |= m_masterVolume.slider->handleMousePressed(mousePos);
                    if (m_musicVolume.slider) handled |= m_musicVolume.slider->handleMousePressed(mousePos);
                    if (m_sfxVolume.slider) handled |= m_sfxVolume.slider->handleMousePressed(mousePos);
                    if (m_languageDropdown) handled |= m_languageDropdown->handleMousePressed(mousePos);
                }
            }

            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (m_masterVolume.slider) m_masterVolume.slider->handleMouseReleased();
                    if (m_musicVolume.slider) m_musicVolume.slider->handleMouseReleased();
                    if (m_sfxVolume.slider) m_sfxVolume.slider->handleMouseReleased();
                }
            }
        }
        catch (const std::exception& e) {
            std::cout << "Error handling event: " << e.what() << std::endl;
        }
    }
}

void SettingsScreen::update(float deltaTime) {
    try {
        m_animationTime += deltaTime;

        if (m_masterVolume.slider) m_masterVolume.slider->update(deltaTime);
        if (m_musicVolume.slider) m_musicVolume.slider->update(deltaTime);
        if (m_sfxVolume.slider) m_sfxVolume.slider->update(deltaTime);
        if (m_languageDropdown) m_languageDropdown->update(deltaTime);

        if (m_settingsChanged && m_autoSaveClock.getElapsedTime().asSeconds() > 2.0f) {
            autoSaveSettings();
            m_settingsChanged = false;
            m_autoSaveClock.restart();
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error in update: " << e.what() << std::endl;
    }
}

void SettingsScreen::render(sf::RenderWindow& window) {
    try {
        if (m_backgroundTexture.getSize().x > 0) {
            window.draw(m_backgroundSprite);
        }

        window.draw(m_titleText);

        window.draw(m_masterVolume.label);
        if (m_masterVolume.slider) m_masterVolume.slider->render(window);
        window.draw(m_masterVolume.value);

        window.draw(m_musicVolume.label);
        if (m_musicVolume.slider) m_musicVolume.slider->render(window);
        window.draw(m_musicVolume.value);

        window.draw(m_sfxVolume.label);
        if (m_sfxVolume.slider) m_sfxVolume.slider->render(window);
        window.draw(m_sfxVolume.value);

        window.draw(m_languageLabel);
        if (m_languageDropdown) m_languageDropdown->render(window);

        window.draw(m_backText);
    }
    catch (const std::exception& e) {
        std::cout << "Error in render: " << e.what() << std::endl;
    }
}

void SettingsScreen::updateVolumeTexts() {
    if (m_masterVolume.slider) {
        m_masterVolume.value.setString(std::to_string(static_cast<int>(m_masterVolume.slider->getValue())) + "%");
    }
    if (m_musicVolume.slider) {
        m_musicVolume.value.setString(std::to_string(static_cast<int>(m_musicVolume.slider->getValue())) + "%");
    }
    if (m_sfxVolume.slider) {
        m_sfxVolume.value.setString(std::to_string(static_cast<int>(m_sfxVolume.slider->getValue())) + "%");
    }
}

void SettingsScreen::updateLanguageTexts() {
    auto& langManager = LanguageManager::instance();

    m_titleText.setString(langManager.getText("settings_title"));

    std::string volumeText = langManager.getText("settings_volume");
    m_masterVolume.label.setString(volumeText + " (Master):");
    m_musicVolume.label.setString(volumeText + " (Music):");
    m_sfxVolume.label.setString(volumeText + " (SFX):");
    m_languageLabel.setString(langManager.getText("settings_language") + ":");
    m_backText.setString(langManager.getText("settings_back"));

    // Update fonts for proper language display
    Language currentLang = langManager.getCurrentLanguage();
    const sf::Font& currentFont = MultiFontManager::instance().getFontForLanguage(currentLang);

    m_titleText.setFont(currentFont);
    m_masterVolume.label.setFont(currentFont);
    m_musicVolume.label.setFont(currentFont);
    m_sfxVolume.label.setFont(currentFont);
    m_languageLabel.setFont(currentFont);
    m_backText.setFont(currentFont);
}

void SettingsScreen::onMasterVolumeChanged(float volume) {
    try {
        AudioManager::instance().setMasterVolume(volume);
        updateVolumeTexts();
        m_settingsChanged = true;
    }
    catch (const std::exception& e) {
        std::cout << "Error changing master volume: " << e.what() << std::endl;
    }
}

void SettingsScreen::onMusicVolumeChanged(float volume) {
    try {
        AudioManager::instance().setMusicVolume(volume);
        updateVolumeTexts();
        m_settingsChanged = true;
    }
    catch (const std::exception& e) {
        std::cout << "Error changing music volume: " << e.what() << std::endl;
    }
}

void SettingsScreen::onSFXVolumeChanged(float volume) {
    try {
        AudioManager::instance().setSFXVolume(volume);
        updateVolumeTexts();
        m_settingsChanged = true;
    }
    catch (const std::exception& e) {
        std::cout << "Error changing SFX volume: " << e.what() << std::endl;
    }
}

void SettingsScreen::onLanguageChanged(int index, int value) {
    try {
        Language newLang = static_cast<Language>(value);
        LanguageManager::instance().setLanguage(newLang);

        // Save language preference
        std::ofstream file("language.txt");
        if (file.is_open()) {
            file << "language=" << value << std::endl;
            file.close();
        }

        // Update all UI text
        updateLanguageTexts();
        m_settingsChanged = true;

        std::cout << "Language changed to: " << value << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Error changing language: " << e.what() << std::endl;
    }
}

void SettingsScreen::autoSaveSettings() {
    try {
        if (m_settingsManager) {
            if (m_masterVolume.slider) {
                m_settingsManager->setSetting("master_volume", m_masterVolume.slider->getValue());
            }
            if (m_musicVolume.slider) {
                m_settingsManager->setSetting("music_volume", m_musicVolume.slider->getValue());
            }
            if (m_sfxVolume.slider) {
                m_settingsManager->setSetting("sfx_volume", m_sfxVolume.slider->getValue());
            }
            m_settingsManager->save();
            std::cout << "Settings auto-saved" << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error auto-saving settings: " << e.what() << std::endl;
    }
}

void SettingsScreen::resetToDefaults() {
    try {
        if (m_settingsManager) {
            m_settingsManager->clear();
            AudioManager::instance().setMasterVolume(100.0f);
            AudioManager::instance().setMusicVolume(100.0f);
            AudioManager::instance().setSFXVolume(100.0f);
            LanguageManager::instance().setLanguage(Language::ENGLISH);
            setupVolumeControls();
            setupLanguageControl();
            updateLanguageTexts();
            std::cout << "Settings reset to defaults" << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error resetting settings: " << e.what() << std::endl;
    }
}

// FileSettingsStorage implementation
void FileSettingsStorage::saveSettings(const std::unordered_map<std::string, float>& settings) {
    try {
        std::ofstream file(m_filename);
        if (file.is_open()) {
            for (const auto& [key, value] : settings) {
                file << key << "=" << value << std::endl;
            }
            file.close();
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error saving settings to file: " << e.what() << std::endl;
    }
}

std::unordered_map<std::string, float> FileSettingsStorage::loadSettings() {
    std::unordered_map<std::string, float> settings;
    try {
        std::ifstream file(m_filename);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                if (line.length() == 0) continue;
                size_t pos = line.find('=');
                if (pos != std::string::npos && pos > 0 && pos < line.length() - 1) {
                    std::string key = line.substr(0, pos);
                    std::string valueStr = line.substr(pos + 1);
                    if (!valueStr.empty()) {
                        float value = std::stof(valueStr);
                        settings[key] = value;
                    }
                }
            }
            file.close();
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error loading settings from file: " << e.what() << std::endl;
    }
    return settings;
}

bool FileSettingsStorage::isValid() const {
    return !m_filename.empty();
}




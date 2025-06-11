#include "VolumeControlPanel.h"
#include "Slider.h"
#include "../Core/AudioManager.h"
#include "../Core/LanguageManager.h"
#include <iostream>

// VolumeSlider implementation
VolumeControlPanel::VolumeSlider::VolumeSlider(const std::string& labelText,
    const std::string& sliderType,
    const sf::Font& font,
    sf::Vector2f position)
    : type(sliderType) {

    // Setup label
    label.setFont(font);
    label.setString(labelText);
    label.setCharacterSize(28);
    label.setFillColor(sf::Color::White);
    label.setPosition(position);

    // Setup slider
    sf::Vector2f sliderPos(position.x + 200, position.y);
    slider = std::make_unique<Slider>(sliderPos, sf::Vector2f(250, 35), 0.0f, 100.0f);

    // Setup value text
    value.setFont(font);
    value.setCharacterSize(24);
    value.setFillColor(sf::Color::Yellow);
    value.setPosition(position.x + 470, position.y + 5);

    updateValueText();
}

void VolumeControlPanel::VolumeSlider::updateValueText() {
    if (slider) {
        value.setString(std::to_string(static_cast<int>(slider->getValue())) + "%");
    }
}

void VolumeControlPanel::VolumeSlider::setVolume(float volume) {
    if (slider) {
        slider->setValue(volume);
        updateValueText();
    }
}

float VolumeControlPanel::VolumeSlider::getVolume() const {
    return slider ? slider->getValue() : 0.0f;
}

// VolumeControlPanel implementation
VolumeControlPanel::VolumeControlPanel(const sf::Font& font)
    : m_position(350, 250) {

    try {
        setupMasterVolumeSlider(font);
        setupMusicVolumeSlider(font);
        setupSFXVolumeSlider(font);

        refreshFromAudioManager();

        std::cout << "VolumeControlPanel initialized successfully" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Error initializing VolumeControlPanel: " << e.what() << std::endl;
    }
}

VolumeControlPanel::~VolumeControlPanel() {
    // Cleanup handled automatically by smart pointers
}

void VolumeControlPanel::setupMasterVolumeSlider(const sf::Font& font) {
    auto& lang = LanguageManager::instance();
    std::string labelText = lang.getText("settings_volume") + " (Master):";

    sf::Vector2f position = calculateSliderPosition(0);
    m_masterVolume = std::make_unique<VolumeSlider>(labelText, "master", font, position);

    // Set callback
    if (m_masterVolume->slider) {
        m_masterVolume->slider->setOnValueChanged([this](float value) {
            onVolumeChanged("master", value);
            });
    }
}

void VolumeControlPanel::setupMusicVolumeSlider(const sf::Font& font) {
    auto& lang = LanguageManager::instance();
    std::string labelText = lang.getText("settings_volume") + " (Music):";

    sf::Vector2f position = calculateSliderPosition(1);
    m_musicVolume = std::make_unique<VolumeSlider>(labelText, "music", font, position);

    // Set callback
    if (m_musicVolume->slider) {
        m_musicVolume->slider->setOnValueChanged([this](float value) {
            onVolumeChanged("music", value);
            });
    }
}

void VolumeControlPanel::setupSFXVolumeSlider(const sf::Font& font) {
    auto& lang = LanguageManager::instance();
    std::string labelText = lang.getText("settings_volume") + " (SFX):";

    sf::Vector2f position = calculateSliderPosition(2);
    m_sfxVolume = std::make_unique<VolumeSlider>(labelText, "sfx", font, position);

    // Set callback
    if (m_sfxVolume->slider) {
        m_sfxVolume->slider->setOnValueChanged([this](float value) {
            onVolumeChanged("sfx", value);
            });
    }
}

void VolumeControlPanel::update(float deltaTime) {
    try {
        if (m_masterVolume && m_masterVolume->slider) {
            m_masterVolume->slider->update(deltaTime);
        }
        if (m_musicVolume && m_musicVolume->slider) {
            m_musicVolume->slider->update(deltaTime);
        }
        if (m_sfxVolume && m_sfxVolume->slider) {
            m_sfxVolume->slider->update(deltaTime);
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error updating VolumeControlPanel: " << e.what() << std::endl;
    }
}

void VolumeControlPanel::render(sf::RenderWindow& window) {
    try {
        // Render master volume
        if (m_masterVolume) {
            window.draw(m_masterVolume->label);
            if (m_masterVolume->slider) {
                m_masterVolume->slider->render(window);
            }
            window.draw(m_masterVolume->value);
        }

        // Render music volume
        if (m_musicVolume) {
            window.draw(m_musicVolume->label);
            if (m_musicVolume->slider) {
                m_musicVolume->slider->render(window);
            }
            window.draw(m_musicVolume->value);
        }

        // Render SFX volume
        if (m_sfxVolume) {
            window.draw(m_sfxVolume->label);
            if (m_sfxVolume->slider) {
                m_sfxVolume->slider->render(window);
            }
            window.draw(m_sfxVolume->value);
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error rendering VolumeControlPanel: " << e.what() << std::endl;
    }
}

bool VolumeControlPanel::handleMouseEvent(const sf::Event& event) {
    try {
        bool handled = false;

        if (event.type == sf::Event::MouseMoved) {
            sf::Vector2f mousePos(static_cast<float>(event.mouseMove.x),
                static_cast<float>(event.mouseMove.y));

            if (m_masterVolume && m_masterVolume->slider) {
                handled |= m_masterVolume->slider->handleMouseMove(mousePos);
            }
            if (m_musicVolume && m_musicVolume->slider) {
                handled |= m_musicVolume->slider->handleMouseMove(mousePos);
            }
            if (m_sfxVolume && m_sfxVolume->slider) {
                handled |= m_sfxVolume->slider->handleMouseMove(mousePos);
            }
        }
        else if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x),
                static_cast<float>(event.mouseButton.y));

            if (m_masterVolume && m_masterVolume->slider) {
                handled |= m_masterVolume->slider->handleMousePressed(mousePos);
            }
            if (m_musicVolume && m_musicVolume->slider) {
                handled |= m_musicVolume->slider->handleMousePressed(mousePos);
            }
            if (m_sfxVolume && m_sfxVolume->slider) {
                handled |= m_sfxVolume->slider->handleMousePressed(mousePos);
            }
        }
        else if (event.type == sf::Event::MouseButtonReleased &&
            event.mouseButton.button == sf::Mouse::Left) {
            if (m_masterVolume && m_masterVolume->slider) {
                handled |= m_masterVolume->slider->handleMouseReleased();
            }
            if (m_musicVolume && m_musicVolume->slider) {
                handled |= m_musicVolume->slider->handleMouseReleased();
            }
            if (m_sfxVolume && m_sfxVolume->slider) {
                handled |= m_sfxVolume->slider->handleMouseReleased();
            }
        }

        return handled;
    }
    catch (const std::exception& e) {
        std::cout << "Error handling mouse event in VolumeControlPanel: " << e.what() << std::endl;
        return false;
    }
}

void VolumeControlPanel::refreshFromAudioManager() {
    try {
        auto& audioManager = AudioManager::instance();

        if (m_masterVolume) {
            m_masterVolume->setVolume(audioManager.getMasterVolume());
        }
        if (m_musicVolume) {
            m_musicVolume->setVolume(audioManager.getMusicVolume());
        }
        if (m_sfxVolume) {
            m_sfxVolume->setVolume(audioManager.getSFXVolume());
        }

        m_hasChanged = false;
        std::cout << "Volume values refreshed from AudioManager" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Error refreshing from AudioManager: " << e.what() << std::endl;
    }
}

void VolumeControlPanel::saveSettings() {
    try {
        // AudioManager saves automatically when values change
        // This method could be extended for additional persistence
        m_hasChanged = false;
        std::cout << "Volume settings saved" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Error saving volume settings: " << e.what() << std::endl;
    }
}

void VolumeControlPanel::onVolumeChanged(const std::string& type, float value) {
    try {
        applyVolumeToAudioManager(type, value);
        updateAllValueTexts();
        m_hasChanged = true;

        std::cout << "Volume changed - " << type << ": " << value << "%" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Error handling volume change: " << e.what() << std::endl;
    }
}

sf::Vector2f VolumeControlPanel::calculateSliderPosition(int index) const {
    return sf::Vector2f(m_position.x, m_position.y + (index * m_spacing));
}

void VolumeControlPanel::updateAllValueTexts() {
    if (m_masterVolume) m_masterVolume->updateValueText();
    if (m_musicVolume) m_musicVolume->updateValueText();
    if (m_sfxVolume) m_sfxVolume->updateValueText();
}

void VolumeControlPanel::applyVolumeToAudioManager(const std::string& type, float value) {
    auto& audioManager = AudioManager::instance();

    if (type == "master") {
        audioManager.setMasterVolume(value);
    }
    else if (type == "music") {
        audioManager.setMusicVolume(value);
    }
    else if (type == "sfx") {
        audioManager.setSFXVolume(value);
    }
    else {
        std::cout << "Unknown volume type: " << type << std::endl;
    }
}
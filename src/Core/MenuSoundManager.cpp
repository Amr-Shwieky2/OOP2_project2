#include "MenuSoundManager.h"
#include <iostream>

MenuSoundManager::MenuSoundManager() {}

bool MenuSoundManager::loadSounds(const std::string& filePath) {
    bool success = true;

    auto loadMusic = [&](const std::string& name) {
        auto music = std::make_unique<sf::Music>();
        if (!music->openFromFile(filePath)) {
            std::cerr << "Failed to load music: " << filePath << std::endl;
            success = false;
            return;
        }
        m_music[name] = std::move(music);
        };

    loadMusic("menu_music");
    loadMusic("loading_music");
    loadMusic("settings_music");
    loadMusic("help_music");
    loadMusic("about_music");

    if (!m_soundBuffers["hover"].loadFromFile(filePath)) success = false;
    if (!m_soundBuffers["click"].loadFromFile(filePath)) success = false;

    m_sounds["hover"].setBuffer(m_soundBuffers["hover"]);
    m_sounds["click"].setBuffer(m_soundBuffers["click"]);

    return success;
}

MenuSoundManager& MenuSoundManager::instance() {
    static MenuSoundManager instance;
    return instance;
}

void MenuSoundManager::playMenuMusic(const std::string& key) {
    if (!m_enabled) return;

    if (m_current) m_current->stop();

    auto it = m_music.find(key);
    if (it != m_music.end()) {
        m_current = it->second.get();
        m_current->setLoop(true);
        m_current->setVolume(m_volume);
        m_current->play();
    }
}

void MenuSoundManager::playButtonHoverSound() {
    if (!m_enabled) return;

    auto& sound = m_sounds["hover"];
    sound.setVolume(m_volume * 0.5f);
    sound.play();
}

void MenuSoundManager::playButtonClickSound() {
    if (!m_enabled) return;

    auto& sound = m_sounds["click"];
    sound.setVolume(m_volume);
    sound.play();
}

void MenuSoundManager::setVolume(float volume) {
    m_volume = std::max(0.0f, std::min(100.0f, volume));
    if (m_current) {
        m_current->setVolume(m_volume);
    }
}

void MenuSoundManager::enable(bool enable) {
    m_enabled = enable;
}

bool MenuSoundManager::isEnabled() const {
    return m_enabled;
}

float MenuSoundManager::getVolume() const {
    return m_volume;
}

void MenuSoundManager::stop() {
    if (m_current) {
        m_current->stop();
        m_current = nullptr;
    }
}

void MenuSoundManager::reset() {
    stop();
    m_volume = 100.0f;
    m_enabled = true;
}

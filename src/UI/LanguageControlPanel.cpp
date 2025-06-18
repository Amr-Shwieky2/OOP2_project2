#include "../UI/LanguageControlPanel.h"
#include "Dropdown.h"
#include "../Core/MultiFontManager.h"
#include <iostream>
#include <fstream>

LanguageControlPanel::LanguageControlPanel(const sf::Font& font)
    : m_position(350, 520) {  // Position below volume controls

    try {
        setupLanguageOptions();
        setupLabel(font);
        setupDropdown(font);
        loadSavedLanguage();
        refreshTexts();

        std::cout << "LanguageControlPanel initialized successfully" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Error initializing LanguageControlPanel: " << e.what() << std::endl;
    }
}

void LanguageControlPanel::setupLanguageOptions() {
    // Clear existing options
    m_availableLanguages.clear();

    // Add supported languages
    m_availableLanguages.emplace_back(Language::ENGLISH, "English", "English", 0);
    m_availableLanguages.emplace_back(Language::ARABIC, "العربية", "Arabic", 1);
    m_availableLanguages.emplace_back(Language::HEBREW, "עברית", "Hebrew", 2);

    // Check which languages are actually supported
    checkLanguageSupport();

    std::cout << "Language options setup completed. Available languages: "
        << m_availableLanguages.size() << std::endl;
}

void LanguageControlPanel::setupLabel(const sf::Font& font) {
    auto& lang = LanguageManager::instance();

    m_labelText.setFont(font);
    m_labelText.setString(lang.getText("settings_language") + ":");
    m_labelText.setCharacterSize(28);
    m_labelText.setFillColor(m_labelColor);
    m_labelText.setPosition(m_position);
}

LanguageControlPanel::~LanguageControlPanel() {
    // std::unique_ptr will automatically clean up m_dropdown
    // since we now have the complete type definition available
}

void LanguageControlPanel::setupDropdown(const sf::Font& font) {
    sf::Vector2f dropdownPos(m_position.x + 200, m_position.y);
    m_dropdown = std::make_unique<Dropdown>(dropdownPos, m_dropdownSize, font);

    // Set dropdown colors
    m_dropdown->setColors(m_dropdownBackgroundColor,
        m_dropdownTextColor,
        m_dropdownHighlightColor);

    // Populate with languages
    populateDropdownWithLanguages();

    // Set selection callback
    m_dropdown->setOnSelectionChanged([this](int index, int value) {
        onLanguageSelectionChanged(index, value);
        });

    // Set current selection
    setSelectedLanguageInDropdown();
}

void LanguageControlPanel::populateDropdownWithLanguages() {
    if (!m_dropdown) return;

    for (const auto& langOption : m_availableLanguages) {
        if (isLanguageSupported(langOption.language)) {
            m_dropdown->addOption(langOption.displayName, langOption.value);
            std::cout << "Added language option: " << langOption.safeName << std::endl;
        }
        else {
            std::cout << "Skipped unsupported language: " << langOption.safeName << std::endl;
        }
    }
}

void LanguageControlPanel::update(float deltaTime) {
    try {
        if (m_dropdown) {
            m_dropdown->update(deltaTime);
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error updating LanguageControlPanel: " << e.what() << std::endl;
    }
}

void LanguageControlPanel::render(sf::RenderWindow& window) {
    try {
        window.draw(m_labelText);

        if (m_dropdown) {
            m_dropdown->render(window);
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error rendering LanguageControlPanel: " << e.what() << std::endl;
    }
}

bool LanguageControlPanel::handleMouseEvent(const sf::Event& event) {
    try {
        if (!m_dropdown) return false;

        bool handled = false;

        if (event.type == sf::Event::MouseMoved) {
            sf::Vector2f mousePos(static_cast<float>(event.mouseMove.x),
                static_cast<float>(event.mouseMove.y));
            handled = m_dropdown->handleMouseMove(mousePos);
        }
        else if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x),
                static_cast<float>(event.mouseButton.y));
            handled = m_dropdown->handleMousePressed(mousePos);
        }

        return handled;
    }
    catch (const std::exception& e) {
        std::cout << "Error handling mouse event in LanguageControlPanel: " << e.what() << std::endl;
        return false;
    }
}

void LanguageControlPanel::refreshTexts() {
    try {
        auto& lang = LanguageManager::instance();

        // Update label text
        m_labelText.setString(lang.getText("settings_language") + ":");

        // Update font if needed for current language
        const sf::Font& currentFont = MultiFontManager::instance().getCurrentFont();
        m_labelText.setFont(currentFont);

        std::cout << "Language control texts refreshed" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Error refreshing language texts: " << e.what() << std::endl;
    }
}

void LanguageControlPanel::onLanguageSelectionChanged(int selectedIndex, int languageValue) {
    try {
        Language newLanguage = getLanguageFromValue(languageValue);
        Language currentLanguage = LanguageManager::instance().getCurrentLanguage();

        if (newLanguage != currentLanguage) {
            std::cout << "Language selection changed to value: " << languageValue << std::endl;

            // Change language
            LanguageManager::instance().setLanguage(newLanguage);

            // Save preference
            saveLanguageToFile(newLanguage);

            // Mark as changed
            m_hasChanged = true;

            // Refresh texts
            refreshTexts();

            std::cout << "Language successfully changed and saved" << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error changing language: " << e.what() << std::endl;
    }
}

void LanguageControlPanel::setSelectedLanguageInDropdown() {
    if (!m_dropdown) return;

    try {
        Language currentLang = LanguageManager::instance().getCurrentLanguage();
        int index = findLanguageIndex(currentLang);

        if (index >= 0) {
            m_dropdown->setSelectedIndex(index);
            std::cout << "Set dropdown selection to index: " << index << std::endl;
        }
        else {
            std::cout << "Could not find current language in dropdown options" << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error setting dropdown selection: " << e.what() << std::endl;
    }
}

int LanguageControlPanel::findLanguageIndex(Language language) const {
    for (size_t i = 0; i < m_availableLanguages.size(); ++i) {
        if (m_availableLanguages[i].language == language &&
            isLanguageSupported(language)) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

Language LanguageControlPanel::getLanguageFromValue(int value) const {
    for (const auto& langOption : m_availableLanguages) {
        if (langOption.value == value) {
            return langOption.language;
        }
    }
    return Language::ENGLISH; // Default fallback
}

void LanguageControlPanel::checkLanguageSupport() {
    // Check font support for each language
    auto& fontManager = MultiFontManager::instance();

    for (const auto& langOption : m_availableLanguages) {
        bool supported = fontManager.isLanguageSupported(langOption.language);
        std::cout << "Language " << langOption.safeName
            << " support: " << (supported ? "YES" : "NO") << std::endl;
    }
}

bool LanguageControlPanel::isLanguageSupported(Language language) const {
    return MultiFontManager::instance().isLanguageSupported(language);
}

void LanguageControlPanel::loadSavedLanguage() {
    try {
        std::ifstream file("language.txt");
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                if (line.empty()) continue;

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
                                std::cout << "Loaded saved language value: " << value << std::endl;
                                break;
                            }
                        }
                        catch (const std::exception& e) {
                            std::cout << "Error parsing saved language: " << e.what() << std::endl;
                        }
                    }
                }
            }
            file.close();
        }
        else {
            std::cout << "No saved language preference found, using default" << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error loading saved language: " << e.what() << std::endl;
    }
}

void LanguageControlPanel::saveLanguageToFile(Language language) {
    try {
        std::ofstream file("language.txt");
        if (file.is_open()) {
            file << "language=" << static_cast<int>(language) << std::endl;
            file.close();
            std::cout << "Language preference saved to file" << std::endl;
        }
        else {
            std::cout << "Failed to open language.txt for writing" << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error saving language to file: " << e.what() << std::endl;
    }
}

void LanguageControlPanel::saveLanguagePreference() {
    try {
        Language currentLang = LanguageManager::instance().getCurrentLanguage();
        saveLanguageToFile(currentLang);
        m_hasChanged = false;
    }
    catch (const std::exception& e) {
        std::cout << "Error saving language preference: " << e.what() << std::endl;
    }
}
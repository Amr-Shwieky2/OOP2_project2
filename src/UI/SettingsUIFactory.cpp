// src/UI/SettingsUIFactory.cpp
#include "SettingsUIFactory.h"
#include "../Core/MultiFontManager.h"
#include <iostream>

sf::Text SettingsUIFactory::createTitle(const sf::String& text, sf::Vector2f position,
    const sf::Font& font, const UITheme::Colors& colors) {
    try {
        sf::Text title;
        title.setFont(font);
        title.setString(text);
        title.setCharacterSize(48);
        title.setFillColor(colors.titleText);
        title.setPosition(position);
        title.setStyle(sf::Text::Bold);
        return title;
    }
    catch (const std::exception& e) {
        std::cout << "Error creating title: " << e.what() << std::endl;
        sf::Text fallback;
        fallback.setString("Settings");
        fallback.setCharacterSize(48);
        fallback.setFillColor(sf::Color::Yellow);
        fallback.setPosition(position);
        return fallback;
    }
}

sf::Text SettingsUIFactory::createLabel(const sf::String& text, sf::Vector2f position,
    const sf::Font& font, const UITheme::Colors& colors, int characterSize) {
    try {
        sf::Text label;
        label.setFont(font);
        label.setString(text);
        label.setCharacterSize(characterSize);
        label.setFillColor(colors.labelText);
        label.setPosition(position);
        return label;
    }
    catch (const std::exception& e) {
        std::cout << "Error creating label: " << e.what() << std::endl;
        sf::Text fallback;
        fallback.setString(text);
        fallback.setCharacterSize(characterSize);
        fallback.setFillColor(sf::Color::White);
        fallback.setPosition(position);
        return fallback;
    }
}

SettingsUIFactory::VolumeControls SettingsUIFactory::createVolumeControl(
    const sf::String& labelText,
    sf::Vector2f position,
    const sf::Font& font,
    const UITheme::Colors& colors,
    const UITheme::Layout& layout,
    float initialValue) {

    VolumeControls controls;

    try {
        // Create label
        controls.label = createLabel(labelText, position, font, colors);

        // Create slider
        controls.slider = std::make_unique<Slider>(
            sf::Vector2f(layout.controlX, position.y + 10),
            layout.sliderSize,
            0.0f,
            100.0f
        );
        controls.slider->setValue(initialValue);
        controls.slider->setColors(colors.sliderBackground, colors.sliderFill, colors.sliderHandle);

        // Create value text
        controls.value = createLabel(
            std::to_string(static_cast<int>(initialValue)) + "%",
            sf::Vector2f(layout.valueX, position.y),
            font,
            colors
        );
        controls.value.setFillColor(colors.valueText);

        std::cout << "Volume control created successfully" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Error creating volume control: " << e.what() << std::endl;
    }

    return controls;
}

std::unique_ptr<Dropdown> SettingsUIFactory::createLanguageDropdown(
    sf::Vector2f position,
    const sf::Font& font,
    const UITheme::Colors& colors,
    const UITheme::Layout& layout) {

    try {
        auto dropdown = std::make_unique<Dropdown>(position, layout.dropdownSize, font);

        // Add language options
        dropdown->addOption(u8"English", static_cast<int>(Language::ENGLISH));
        dropdown->addOption(u8"العربية", static_cast<int>(Language::ARABIC));  // Arabic
        dropdown->addOption(u8"עברית", static_cast<int>(Language::HEBREW));   // Hebrew

        // Set colors
        dropdown->setColors(colors.dropdownBackground, colors.dropdownText, colors.dropdownHighlight);

        // Set current selection based on current language
        Language currentLang = LanguageManager::instance().getCurrentLanguage();
        dropdown->setSelectedIndex(static_cast<int>(currentLang));

        std::cout << "Language dropdown created successfully" << std::endl;
        return dropdown;
    }
    catch (const std::exception& e) {
        std::cout << "Error creating language dropdown: " << e.what() << std::endl;
        return nullptr;
    }
}

// NEW FUNCTIONS IMPLEMENTATION:

sf::Text SettingsUIFactory::createMultilingualText(const sf::String& text,
    sf::Vector2f position,
    Language language,
    const UITheme::Colors& colors,
    int characterSize) {

    try {
        const sf::Font& font = MultiFontManager::instance().getFontForLanguage(language);
        sf::Text multiText;
        multiText.setFont(font);
        multiText.setString(text);
        multiText.setCharacterSize(characterSize);
        multiText.setFillColor(colors.labelText);
        multiText.setPosition(position);

        std::cout << "Multilingual text created for language: "
            << MultiFontManager::instance().getLanguageNameSafe(language) << std::endl;
        return multiText;
    }
    catch (const std::exception& e) {
        std::cout << "Error creating multilingual text: " << e.what() << std::endl;
        // Fallback to default font
        sf::Text fallback;
        fallback.setString(text);
        fallback.setCharacterSize(characterSize);
        fallback.setFillColor(colors.labelText);
        fallback.setPosition(position);
        return fallback;
    }
}

void SettingsUIFactory::updateTextFont(sf::Text& text, Language language) {
    try {
        const sf::Font& font = MultiFontManager::instance().getFontForLanguage(language);
        text.setFont(font);
        std::cout << "Text font updated for language: "
            << MultiFontManager::instance().getLanguageNameSafe(language) << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Error updating text font: " << e.what() << std::endl;
    }
}

std::unique_ptr<Dropdown> SettingsUIFactory::createEnhancedLanguageDropdown(
    sf::Vector2f position,
    const UITheme::Colors& colors,
    const UITheme::Layout& layout) {

    try {
        // Get supported languages from MultiFontManager
        auto supportedLanguages = MultiFontManager::instance().getSupportedLanguages();

        if (supportedLanguages.empty()) {
            std::cout << "No supported languages found!" << std::endl;
            return nullptr;
        }

        // Use the font for current language
        const sf::Font& font = MultiFontManager::instance().getCurrentFont();
        auto dropdown = std::make_unique<Dropdown>(position, layout.dropdownSize, font);

        // Add only supported languages
        int currentIndex = 0;
        Language currentLang = LanguageManager::instance().getCurrentLanguage();

        for (size_t i = 0; i < supportedLanguages.size(); ++i) {
            Language lang = supportedLanguages[i];
            std::string displayText = getLanguageDisplayText(lang);
            std::u8string utf8Text(displayText.begin(), displayText.end()); 
            dropdown->addOption(utf8Text, static_cast<int>(lang));

            if (lang == currentLang) {
                currentIndex = static_cast<int>(i);
            }
        }

        dropdown->setColors(colors.dropdownBackground, colors.dropdownText, colors.dropdownHighlight);
        dropdown->setSelectedIndex(currentIndex);

        std::cout << "Enhanced language dropdown created with "
            << supportedLanguages.size() << " supported languages" << std::endl;
        return dropdown;
    }
    catch (const std::exception& e) {
        std::cout << "Error creating enhanced language dropdown: " << e.what() << std::endl;
        return nullptr;
    }
}

std::string SettingsUIFactory::getLanguageDisplayText(Language language) {
    try {
        return MultiFontManager::instance().getLanguageDisplayName(language);
    }
    catch (const std::exception& e) {
        std::cout << "Error getting language display text: " << e.what() << std::endl;
        // Fallback to safe names
        switch (language) {
        case Language::ENGLISH: return "English";
        case Language::ARABIC: return "Arabic";
        case Language::HEBREW: return "Hebrew";
        default: return "Unknown";
        }
    }
}
// Add these new functions to your existing SettingsUIFactory.cpp

#include "SettingsUIFactory.h"
#include "../Core/MultiFontManager.h"  // Add this include

// NEW FUNCTION 1: Create text with appropriate font for specific language
sf::Text SettingsUIFactory::createMultilingualText(
    const sf::String& text,
    sf::Vector2f position,
    Language language,
    const UITheme::Colors& colors,
    int characterSize
) {
    sf::Text textObject;

    // Use appropriate font for language
    const sf::Font& font = MultiFontManager::instance().getFontForLanguage(language);
    textObject.setFont(font);

    textObject.setString(text);
    textObject.setCharacterSize(characterSize);
    textObject.setFillColor(colors.labelText);
    textObject.setStyle(sf::Text::Bold);
    textObject.setPosition(position);

    return textObject;
}

// NEW FUNCTION 2: Update existing text with appropriate font for language
void SettingsUIFactory::updateTextFont(sf::Text& text, Language language) {
    const sf::Font& font = MultiFontManager::instance().getFontForLanguage(language);
    text.setFont(font);
}

// NEW FUNCTION 3: Create enhanced language dropdown with proper font support
std::unique_ptr<Dropdown> SettingsUIFactory::createEnhancedLanguageDropdown(
    sf::Vector2f position,
    const UITheme::Colors& colors,
    const UITheme::Layout& layout
) {
    // Use current font from MultiFontManager
    const sf::Font& currentFont = MultiFontManager::instance().getCurrentFont();

    auto dropdown = std::make_unique<Dropdown>(
        position,
        layout.dropdownSize,
        currentFont
    );

    dropdown->setColors(
        colors.dropdownBackground,
        colors.dropdownText,
        colors.dropdownHighlight
    );

    // Get supported languages from MultiFontManager
    auto supportedLanguages = MultiFontManager::instance().getSupportedLanguages();

    // Add options based on supported languages
    for (Language lang : supportedLanguages) {
        std::string displayText = getLanguageDisplayText(lang);
        dropdown->addOption(displayText, static_cast<int>(lang));
    }

    // Set current language as selected
    Language currentLang = LanguageManager::instance().getCurrentLanguage();

    // Find appropriate index
    int currentIndex = 0;
    for (size_t i = 0; i < supportedLanguages.size(); i++) {
        if (supportedLanguages[i] == currentLang) {
            currentIndex = static_cast<int>(i);
            break;
        }
    }

    dropdown->setSelectedIndex(currentIndex);

    return dropdown;
}

// NEW FUNCTION 4: Helper function to get display text for language
std::string SettingsUIFactory::getLanguageDisplayText(Language language) {
    switch (language) {
    case Language::ENGLISH:
        return "English";
    case Language::ARABIC:
        // Check if Arabic font is supported
        if (MultiFontManager::instance().isLanguageSupported(Language::ARABIC)) {
            return "العربية"; // Arabic in Arabic script
        }
        else {
            return "Arabic (عربي)"; // Fallback with mixed script
        }
    case Language::HEBREW:
        // Check if Hebrew font is supported
        if (MultiFontManager::instance().isLanguageSupported(Language::HEBREW)) {
            return "עברית"; // Hebrew in Hebrew script
        }
        else {
            return "Hebrew (עברית)"; // Fallback with mixed script
        }
    default:
        return "Unknown";
    }
}

// UPDATED FUNCTION: Enhanced createLanguageDropdown (replaces existing one)
std::unique_ptr<Dropdown> SettingsUIFactory::createLanguageDropdown(
    sf::Vector2f position,
    const sf::Font& font,
    const UITheme::Colors& colors,
    const UITheme::Layout& layout
) {
    auto dropdown = std::make_unique<Dropdown>(
        position,
        layout.dropdownSize,
        font
    );

    dropdown->setColors(
        colors.dropdownBackground,
        colors.dropdownText,
        colors.dropdownHighlight
    );

    // Use helper function for consistent display text
    dropdown->addOption(getLanguageDisplayText(Language::ENGLISH), static_cast<int>(Language::ENGLISH));
    dropdown->addOption(getLanguageDisplayText(Language::ARABIC), static_cast<int>(Language::ARABIC));
    dropdown->addOption(getLanguageDisplayText(Language::HEBREW), static_cast<int>(Language::HEBREW));

    // Set current language as selected
    Language currentLang = LanguageManager::instance().getCurrentLanguage();
    dropdown->setSelectedIndex(static_cast<int>(currentLang));

    return dropdown;
}
// Add this missing function to your SettingsUIFactory.cpp

sf::Text SettingsUIFactory::createLabel(
    const sf::String& text,
    sf::Vector2f position,
    const sf::Font& font,
    const UITheme::Colors& colors,
    int characterSize
) {
    sf::Text label;
    label.setFont(font);
    label.setString(text);
    label.setCharacterSize(characterSize);
    label.setFillColor(colors.labelText);
    label.setStyle(sf::Text::Bold);
    label.setPosition(position);

    return label;
}

// Also add createTitle if missing:
sf::Text SettingsUIFactory::createTitle(
    const sf::String& text,
    sf::Vector2f position,
    const sf::Font& font,
    const UITheme::Colors& colors
) {
    sf::Text title;
    title.setFont(font);
    title.setString(text);
    title.setCharacterSize(48);
    title.setFillColor(colors.titleText);
    title.setStyle(sf::Text::Bold);
    title.setPosition(position);

    // Center the title
    sf::FloatRect textBounds = title.getLocalBounds();
    title.setPosition(position.x - textBounds.width / 2, position.y);

    return title;
}
// Correct createVolumeControl function for SettingsUIFactory.cpp

SettingsUIFactory::VolumeControls SettingsUIFactory::createVolumeControl(
    const sf::String& labelText,
    sf::Vector2f position,
    const sf::Font& font,
    const UITheme::Colors& colors,
    const UITheme::Layout& layout,
    float initialValue
) {
    VolumeControls controls;

    // Create label
    controls.label = createLabel(
        labelText,
        position,
        font,
        colors,
        28
    );

    // Create slider with correct constructor (position, size, minValue, maxValue)
    controls.slider = std::make_unique<Slider>(
        sf::Vector2f(layout.controlX, position.y),  // position
        layout.sliderSize,                          // size
        0.0f,                                      // minValue
        100.0f                                     // maxValue
    );

    // Set initial value
    controls.slider->setValue(initialValue);

    // Create value text
    controls.value = createLabel(
        std::to_string(static_cast<int>(initialValue)) + "%",
        sf::Vector2f(layout.controlX + layout.sliderSize.x + 20, position.y),
        font,
        colors,
        24
    );

    return controls;
}
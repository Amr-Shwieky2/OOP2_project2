#include "SettingsUIFactory.h"
#include "Slider.h"
#include "Dropdown.h"
#include "../Core/LanguageManager.h"
#include "../Core/MultiFontManager.h"

sf::Text SettingsUIFactory::createTitle(const sf::String& text, sf::Vector2f position,
    const sf::Font& font, const UITheme::Colors& colors) {

    sf::Text title;
    title.setFont(font);
    title.setString(text);
    title.setCharacterSize(48);
    title.setFillColor(colors.titleText);
    title.setPosition(position);
    title.setStyle(sf::Text::Bold);

    return title;
}

sf::Text SettingsUIFactory::createLabel(const sf::String& text, sf::Vector2f position,
    const sf::Font& font, const UITheme::Colors& colors, int characterSize) {

    sf::Text label;
    label.setFont(font);
    label.setString(text);
    label.setCharacterSize(characterSize);
    label.setFillColor(colors.labelText);
    label.setPosition(position);

    return label;
}

SettingsUIFactory::VolumeControls SettingsUIFactory::createVolumeControl(
    const sf::String& labelText,
    sf::Vector2f position,
    const sf::Font& font,
    const UITheme::Colors& colors,
    const UITheme::Layout& layout,
    float initialValue) {

    VolumeControls control;

    // Create label
    control.label = createLabel(labelText, position, font, colors);

    // Create slider
    sf::Vector2f sliderPos(position.x + 200, position.y);
    control.slider = std::make_unique<Slider>(sliderPos, layout.sliderSize, 0.0f, 100.0f);
    control.slider->setValue(initialValue);
    control.slider->setColors(colors.sliderBackground, colors.sliderFill, colors.sliderHandle);

    // Create value text
    control.value.setFont(font);
    control.value.setCharacterSize(24);
    control.value.setFillColor(colors.valueText);
    control.value.setPosition(position.x + 470, position.y + 5);
    control.value.setString(std::to_string(static_cast<int>(initialValue)) + "%");

    return control;
}

std::unique_ptr<Dropdown> SettingsUIFactory::createLanguageDropdown(sf::Vector2f position,
    const sf::Font& font,
    const UITheme::Colors& colors,
    const UITheme::Layout& layout) {

    auto dropdown = std::make_unique<Dropdown>(position, layout.dropdownSize, font);
    dropdown->setColors(colors.dropdownBackground, colors.dropdownText, colors.dropdownHighlight);

    return dropdown;
}

// New enhanced functions
sf::Text SettingsUIFactory::createMultilingualText(const sf::String& text,
    sf::Vector2f position,
    Language language,
    const UITheme::Colors& colors,
    int characterSize) {

    sf::Text mlText;

    // Get appropriate font for language
    const sf::Font& font = MultiFontManager::instance().getFontForLanguage(language);
    mlText.setFont(font);

    mlText.setString(text);
    mlText.setCharacterSize(characterSize);
    mlText.setFillColor(colors.labelText);
    mlText.setPosition(position);

    return mlText;
}

void SettingsUIFactory::updateTextFont(sf::Text& text, Language language) {
    const sf::Font& font = MultiFontManager::instance().getFontForLanguage(language);
    text.setFont(font);
}

std::unique_ptr<Dropdown> SettingsUIFactory::createEnhancedLanguageDropdown(sf::Vector2f position,
    const UITheme::Colors& colors,
    const UITheme::Layout& layout) {

    // Get current font
    const sf::Font& font = MultiFontManager::instance().getCurrentFont();

    auto dropdown = std::make_unique<Dropdown>(position, layout.dropdownSize, font);
    dropdown->setColors(colors.dropdownBackground, colors.dropdownText, colors.dropdownHighlight);

    return dropdown;
}

std::string SettingsUIFactory::getLanguageDisplayText(Language language) {
    return MultiFontManager::instance().getLanguageDisplayName(language);
}
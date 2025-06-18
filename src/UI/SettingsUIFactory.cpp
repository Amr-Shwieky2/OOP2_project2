#include "SettingsUIFactory.h"
#include "../Core/LanguageManager.h"

SettingsUIFactory::VolumeControls SettingsUIFactory::createVolumeControl(
    const std::string& labelText,
    sf::Vector2f position,
    const sf::Font& font,
    const UITheme::Colors& colors,
    const UITheme::Layout& layout,
    float currentValue
) {
    VolumeControls controls;
    
    // Create label
    controls.label = createLabel(labelText, position, font, colors);
    
    // Create slider
    controls.slider = std::make_unique<Slider>(
        sf::Vector2f(layout.controlX, position.y + 10),
        layout.sliderSize,
        0.0f, 100.0f
    );
    controls.slider->setValue(currentValue);
    controls.slider->setColors(
        colors.sliderBackground,
        colors.sliderFill,
        colors.sliderHandle
    );
    
    // Create value text
    controls.value.setFont(font);
    controls.value.setCharacterSize(24);
    controls.value.setFillColor(colors.valueText);
    controls.value.setStyle(sf::Text::Bold);
    controls.value.setPosition(layout.valueX, position.y + 2);
    controls.value.setString(std::to_string(static_cast<int>(currentValue)) + "%");
    
    return controls;
}

std::unique_ptr<Dropdown> SettingsUIFactory::createLanguageDropdown(
    sf::Vector2f position,
    const sf::Font& font,
    const UITheme::Colors& colors,
    const UITheme::Layout& layout
) {
    auto dropdown = std::make_unique<Dropdown>(
        sf::Vector2f(layout.controlX, position.y),
        layout.dropdownSize,
        font
    );
    
    dropdown->setColors(
        colors.dropdownBackground,
        colors.dropdownText,
        colors.dropdownHighlight
    );
    
    // Add language options
    dropdown->addOption("English", static_cast<int>(Language::ENGLISH));
    dropdown->addOption("עברית", static_cast<int>(Language::HEBREW));
    dropdown->addOption("العربية", static_cast<int>(Language::ARABIC));
    
    // Set current language
    Language currentLang = LanguageManager::instance().getCurrentLanguage();
    dropdown->setSelectedIndex(static_cast<int>(currentLang));
    
    return dropdown;
}

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
    return title;
}
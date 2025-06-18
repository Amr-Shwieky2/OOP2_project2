#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "Slider.h"
#include "Dropdown.h"
#include "../Core/UITheme.h"

class SettingsUIFactory {
public:
    struct VolumeControls {
        sf::Text label;
        std::unique_ptr<Slider> slider;
        sf::Text value;
    };

    static VolumeControls createVolumeControl(
        const std::string& labelText,
        sf::Vector2f position,
        const sf::Font& font,
        const UITheme::Colors& colors,
        const UITheme::Layout& layout,
        float currentValue
    );

    static std::unique_ptr<Dropdown> createLanguageDropdown(
        sf::Vector2f position,
        const sf::Font& font,
        const UITheme::Colors& colors,
        const UITheme::Layout& layout
    );

    static sf::Text createLabel(
        const sf::String& text,
        sf::Vector2f position,
        const sf::Font& font,
        const UITheme::Colors& colors,
        int characterSize = 45
    );

    static sf::Text createTitle(
        const sf::String& text,
        sf::Vector2f position,
        const sf::Font& font,
        const UITheme::Colors& colors
    );
};

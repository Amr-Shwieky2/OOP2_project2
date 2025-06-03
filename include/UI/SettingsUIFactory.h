// Add these new functions to your existing SettingsUIFactory.h

#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "../Core/UITheme.h"
#include "../Core/LanguageManager.h"  // Add this include
#include "Dropdown.h"
#include "Slider.h"

class SettingsUIFactory {
public:
    struct VolumeControls {
        sf::Text label;
        std::unique_ptr<Slider> slider;
        sf::Text value;
    };

    // Existing functions (keep as they are)
    static sf::Text createTitle(const sf::String& text, sf::Vector2f position,
        const sf::Font& font, const UITheme::Colors& colors);

    static sf::Text createLabel(const sf::String& text, sf::Vector2f position,
        const sf::Font& font, const UITheme::Colors& colors,
        int characterSize = 28);

    static VolumeControls createVolumeControl(const sf::String& labelText,
        sf::Vector2f position,
        const sf::Font& font,
        const UITheme::Colors& colors,
        const UITheme::Layout& layout,
        float initialValue = 100.0f);

    static std::unique_ptr<Dropdown> createLanguageDropdown(sf::Vector2f position,
        const sf::Font& font,
        const UITheme::Colors& colors,
        const UITheme::Layout& layout);

    // NEW FUNCTIONS - Add these to your existing header:

    // Create text with appropriate font for specific language
    static sf::Text createMultilingualText(const sf::String& text,
        sf::Vector2f position,
        Language language,
        const UITheme::Colors& colors,
        int characterSize = 28);

    // Update existing text with appropriate font for language
    static void updateTextFont(sf::Text& text, Language language);

    // Create enhanced language dropdown with proper font support
    static std::unique_ptr<Dropdown> createEnhancedLanguageDropdown(sf::Vector2f position,
        const UITheme::Colors& colors,
        const UITheme::Layout& layout);

    // Helper function to get display text for language
    static std::string getLanguageDisplayText(Language language);

private:
    SettingsUIFactory() = default;
};
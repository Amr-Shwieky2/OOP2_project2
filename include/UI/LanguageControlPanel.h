#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "../Core/LanguageManager.h"

// Forward declarations
class Dropdown;

/**
 * Language Control Panel - handles only language selection
 * Single Responsibility: Language management
 */
class LanguageControlPanel {
public:
    explicit LanguageControlPanel(const sf::Font& font);
    ~LanguageControlPanel();
    // Core functionality
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    bool handleMouseEvent(const sf::Event& event);

    // Language management
    void refreshTexts();
    void saveLanguagePreference();

    // State queries
    bool hasChanged() const { return m_hasChanged; }
    void markSaved() { m_hasChanged = false; }

    // Layout configuration
    void setPosition(sf::Vector2f position) { m_position = position; }
    void setDropdownSize(sf::Vector2f size) { m_dropdownSize = size; }

private:
    // Language option structure
    struct LanguageOption {
        Language language;
        std::string displayName;
        std::string safeName;  // For console output
        int value;

        LanguageOption(Language lang, const std::string& display,
            const std::string& safe, int val)
            : language(lang), displayName(display), safeName(safe), value(val) {
        }
    };

    // UI Components
    sf::Text m_labelText;
    std::unique_ptr<Dropdown> m_dropdown;

    // Available languages
    std::vector<LanguageOption> m_availableLanguages;

    // Panel properties
    sf::Vector2f m_position;
    sf::Vector2f m_dropdownSize = sf::Vector2f(200, 60);
    bool m_hasChanged = false;

    // Visual styling
    sf::Color m_labelColor = sf::Color(139, 69, 19);              // Saddle Brown للتسمية
    sf::Color m_dropdownBackgroundColor = sf::Color(245, 245, 220); // Beige للخلفية
    sf::Color m_dropdownTextColor = sf::Color(139, 69, 19);       // Dark Brown للنص
    sf::Color m_dropdownHighlightColor = sf::Color(210, 180, 140); // Tan للتمييز

    // Setup methods
    void setupLanguageOptions();
    void setupDropdown(const sf::Font& font);
    void setupLabel(const sf::Font& font);

    // Event handling
    void onLanguageSelectionChanged(int selectedIndex, int languageValue);

    // Helper methods
    void populateDropdownWithLanguages();
    void setSelectedLanguageInDropdown();
    int findLanguageIndex(Language language) const;
    Language getLanguageFromValue(int value) const;

    // Language support checking
    void checkLanguageSupport();
    bool isLanguageSupported(Language language) const;

    // File I/O
    void loadSavedLanguage();
    void saveLanguageToFile(Language language);
};
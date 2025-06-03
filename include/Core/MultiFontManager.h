#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <memory>
#include <string>
#include <vector>
#include "LanguageManager.h"

class MultiFontManager {
public:
    static MultiFontManager& instance();

    // Load fonts for different languages
    bool loadLanguageFonts();

    // Get font for specific language
    const sf::Font& getFontForLanguage(Language language) const;

    // Get current font based on active language
    const sf::Font& getCurrentFont() const;

    // Check if a language is supported with proper font
    bool isLanguageSupported(Language language) const;

    // Get available languages that have proper font support
    std::vector<Language> getSupportedLanguages() const;

private:
    MultiFontManager() = default;

    std::unordered_map<Language, std::unique_ptr<sf::Font>> m_languageFonts;
    mutable sf::Font m_fallbackFont; // For unsupported languages

    bool loadFontSafely(Language language, const std::string& fontPath);
    void setupFallbackFont();
};

// Enhanced LanguageManager with font support
class EnhancedLanguageManager : public LanguageManager {
public:
    static EnhancedLanguageManager& instance();

    // Override setLanguage to handle font switching
    void setLanguage(Language lang) ;

    // Get text with proper font support check
    const std::string& getTextSafe(const std::string& key) const;

    // Check if current language needs special font
    bool currentLanguageNeedsSpecialFont() const;

    // Get display name for language in its own script
    std::string getLanguageDisplayName(Language lang) const;

private:
    EnhancedLanguageManager() = default;
};
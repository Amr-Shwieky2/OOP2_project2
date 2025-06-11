// src/Core/MultiFontManager.cpp - CONSOLE-SAFE VERSION
#include "MultiFontManager.h"
#include <iostream>
#include <filesystem>

MultiFontManager& MultiFontManager::instance() {
    static MultiFontManager instance;
    return instance;
}

bool MultiFontManager::loadLanguageFonts() {
    bool success = true;

    // Setup fallback font first
    setupFallbackFont();

    std::cout << "Loading language fonts..." << std::endl;

    // English - Try multiple common fonts
    if (!loadFontSafely(Language::ENGLISH, "arial.ttf")) {
        std::cout << "Warning: No specific English font found, using fallback" << std::endl;
        // For English, we can use the fallback font
        m_languageFonts[Language::ENGLISH] = std::make_unique<sf::Font>(m_fallbackFont);
    }

    // Arabic - Try multiple Arabic fonts with various common locations
    if (!loadFontSafely(Language::ARABIC, "NotoSansArabic-Regular.ttf") &&
        !loadFontSafely(Language::ARABIC, "assets/fonts/NotoSansArabic-Regular.ttf") &&
        !loadFontSafely(Language::ARABIC, "fonts/NotoSansArabic.ttf") &&
        !loadFontSafely(Language::ARABIC, "assets/fonts/NotoSansArabic.ttf") &&
        !loadFontSafely(Language::ARABIC, "fonts/Arial_Unicode_MS.ttf") &&
        !loadFontSafely(Language::ARABIC, "assets/fonts/Arial_Unicode_MS.ttf") &&
        !loadFontSafely(Language::ARABIC, "fonts/Tahoma.ttf") &&
        !loadFontSafely(Language::ARABIC, "Tahoma.ttf") &&
        !loadFontSafely(Language::ARABIC, "fonts/Amiri-Regular.ttf") &&
        !loadFontSafely(Language::ARABIC, "assets/fonts/Amiri-Regular.ttf") &&
        !loadFontSafely(Language::ARABIC, "fonts/Scheherazade-Regular.ttf") &&
        !loadFontSafely(Language::ARABIC, "assets/fonts/Scheherazade-Regular.ttf")) {

        std::cout << "Warning: No Arabic font found. Arabic text may display as squares." << std::endl;
        std::cout << "Please download and place one of these fonts in fonts/ or assets/fonts/:" << std::endl;
        std::cout << "  - NotoSansArabic-Regular.ttf (recommended)" << std::endl;
        std::cout << "  - Amiri-Regular.ttf" << std::endl;
        std::cout << "  - Scheherazade-Regular.ttf" << std::endl;
        success = false;
    }

    // Hebrew - Try multiple Hebrew fonts
    if (!loadFontSafely(Language::HEBREW, "NotoSansHebrew-Regular.ttf") &&
        !loadFontSafely(Language::HEBREW, "assets/fonts/NotoSansHebrew-Regular.ttf") &&
        !loadFontSafely(Language::HEBREW, "fonts/NotoSansHebrew.ttf") &&
        !loadFontSafely(Language::HEBREW, "assets/fonts/NotoSansHebrew.ttf") &&
        !loadFontSafely(Language::HEBREW, "fonts/David.ttf") &&
        !loadFontSafely(Language::HEBREW, "assets/fonts/David.ttf") &&
        !loadFontSafely(Language::HEBREW, "fonts/FrankRuehl.ttf") &&
        !loadFontSafely(Language::HEBREW, "assets/fonts/FrankRuehl.ttf") &&
        !loadFontSafely(Language::HEBREW, "fonts/Arial_Unicode_MS.ttf") &&
        !loadFontSafely(Language::HEBREW, "assets/fonts/Arial_Unicode_MS.ttf")) {

        std::cout << "Warning: No Hebrew font found. Hebrew text may display as squares." << std::endl;
        std::cout << "Please download and place one of these fonts in fonts/ or assets/fonts/:" << std::endl;
        std::cout << "  - NotoSansHebrew-Regular.ttf (recommended)" << std::endl;
        std::cout << "  - David.ttf" << std::endl;
        std::cout << "  - FrankRuehl.ttf" << std::endl;
        success = false;
    }

    // Print summary with SAFE language names for console
    auto supportedLanguages = getSupportedLanguages();
    std::cout << "Font loading completed. Supported languages: " << supportedLanguages.size() << std::endl;

    for (Language lang : supportedLanguages) {
        std::cout << "  - " << getLanguageNameSafe(lang) << std::endl;
    }

    return success;
}

bool MultiFontManager::loadFontSafely(Language language, const std::string& fontPath) {
    // Check if file exists first
    if (!std::filesystem::exists(fontPath)) {
        return false;
    }

    auto font = std::make_unique<sf::Font>();
    if (font->loadFromFile(fontPath)) {
        m_languageFonts[language] = std::move(font);
        std::cout << "✓ Loaded font for " << getLanguageNameSafe(language) << ": " << fontPath << std::endl;
        return true;
    }

    std::cout << "✗ Failed to load font: " << fontPath << std::endl;
    return false;
}

void MultiFontManager::setupFallbackFont() {
    // Try to load a basic fallback font
    if (!m_fallbackFont.loadFromFile("arial.ttf") &&
        !m_fallbackFont.loadFromFile("assets/fonts/arial.ttf") &&
        !m_fallbackFont.loadFromFile("arial.ttf")) {
        // If no external font, we'll use SFML's default font
        std::cout << "Using SFML default font as fallback" << std::endl;
    }
    else {
        std::cout << "Loaded fallback font successfully" << std::endl;
    }
}

const sf::Font& MultiFontManager::getFontForLanguage(Language language) const {
    auto it = m_languageFonts.find(language);
    if (it != m_languageFonts.end()) {
        return *it->second;
    }

    // If specific language font not found, try to use a font that supports the script
    if (language == Language::ARABIC || language == Language::HEBREW) {
        // Try to find a font that supports Arabic or Hebrew script
        for (const auto& [lang, font] : m_languageFonts) {
            if (lang == Language::ARABIC || lang == Language::HEBREW) {
                std::cout << "Using cross-language font for " << getLanguageNameSafe(language) << std::endl;
                return *font;
            }
        }
    }

    return m_fallbackFont;
}

const sf::Font& MultiFontManager::getCurrentFont() const {
    Language currentLang = LanguageManager::instance().getCurrentLanguage();
    return getFontForLanguage(currentLang);
}

bool MultiFontManager::isLanguageSupported(Language language) const {
    return m_languageFonts.find(language) != m_languageFonts.end();
}

std::vector<Language> MultiFontManager::getSupportedLanguages() const {
    std::vector<Language> supported;

    // Always include English as it can use fallback
    supported.push_back(Language::ENGLISH);

    // Add others only if they have proper fonts
    if (isLanguageSupported(Language::ARABIC)) {
        supported.push_back(Language::ARABIC);
    }
    if (isLanguageSupported(Language::HEBREW)) {
        supported.push_back(Language::HEBREW);
    }

    return supported;
}

// SAFE function for console output - no Unicode
std::string MultiFontManager::getLanguageNameSafe(Language language) const {
    switch (language) {
    case Language::ENGLISH:
        return "English";
    case Language::ARABIC:
        return "Arabic";  // Safe for console
    case Language::HEBREW:
        return "Hebrew";  // Safe for console
    default:
        return "Unknown";
    }
}

// ORIGINAL function for UI display - with Unicode
std::string MultiFontManager::getLanguageDisplayName(Language language) const {
    switch (language) {
    case Language::ENGLISH:
        return "English";
    case Language::ARABIC:
        return "العربية"; // Arabic in Arabic script - for UI only
    case Language::HEBREW:
        return "עברית"; // Hebrew in Hebrew script - for UI only
    default:
        return "Unknown";
    }
}

// Enhanced LanguageManager Implementation
EnhancedLanguageManager& EnhancedLanguageManager::instance() {
    static EnhancedLanguageManager instance;
    return instance;
}

void EnhancedLanguageManager::setLanguage(Language lang) {
    // Check if the language is supported with proper fonts
    if (!MultiFontManager::instance().isLanguageSupported(lang)) {
        std::cout << "Warning: Language " << MultiFontManager::instance().getLanguageNameSafe(lang)
            << " may not display correctly due to missing fonts" << std::endl;

        // If it's Arabic or Hebrew without proper fonts, suggest staying with English
        if ((lang == Language::ARABIC || lang == Language::HEBREW)) {
            std::cout << "Consider downloading appropriate fonts for better display" << std::endl;
        }
    }

    // Call parent implementation
    LanguageManager::setLanguage(lang);

    std::cout << "Language changed to: " << MultiFontManager::instance().getLanguageNameSafe(lang) << std::endl;
}

const std::string& EnhancedLanguageManager::getTextSafe(const std::string& key) const {
    const std::string& text = getText(key);

    // If text is empty, check if it's a font issue
    if (text.empty() && !MultiFontManager::instance().isLanguageSupported(getCurrentLanguage())) {
        std::cout << "Warning: Text for key '" << key
            << "' may be missing due to unsupported language" << std::endl;
    }

    return text;
}

bool EnhancedLanguageManager::currentLanguageNeedsSpecialFont() const {
    Language currentLang = getCurrentLanguage();
    return currentLang == Language::ARABIC || currentLang == Language::HEBREW;
}

std::string EnhancedLanguageManager::getLanguageDisplayName(Language lang) const {
    return MultiFontManager::instance().getLanguageDisplayName(lang);
}
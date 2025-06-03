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

    // Try to load fonts for each language
    std::cout << "Loading language fonts..." << std::endl;

    // English - Try multiple common fonts
    if (!loadFontSafely(Language::ENGLISH, "fonts/arial.ttf") &&
        !loadFontSafely(Language::ENGLISH, "arial.ttf") &&
        !loadFontSafely(Language::ENGLISH, "fonts/DejaVuSans.ttf")) {
        std::cout << "Warning: No specific English font found, using fallback" << std::endl;
    }

    // Arabic - Try Arabic fonts
    if (!loadFontSafely(Language::ARABIC, "fonts/NotoSansArabic.ttf") &&
        !loadFontSafely(Language::ARABIC, "fonts/Arial_Unicode_MS.ttf") &&
        !loadFontSafely(Language::ARABIC, "fonts/Tahoma.ttf")) {
        std::cout << "Warning: No Arabic font found, Arabic text may not display correctly" << std::endl;
        success = false;
    }

    // Hebrew - Try Hebrew fonts
    if (!loadFontSafely(Language::HEBREW, "fonts/NotoSansHebrew.ttf") &&
        !loadFontSafely(Language::HEBREW, "fonts/David.ttf")) {
        std::cout << "Warning: No Hebrew font found, Hebrew text may not display correctly" << std::endl;
        success = false;
    }

    std::cout << "Font loading completed. Supported languages: " << getSupportedLanguages().size() << std::endl;
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
        std::cout << "Loaded font for " << static_cast<int>(language) << ": " << fontPath << std::endl;
        return true;
    }

    std::cout << "Failed to load font: " << fontPath << std::endl;
    return false;
}

void MultiFontManager::setupFallbackFont() {
    // Try to load a basic fallback font
    if (!m_fallbackFont.loadFromFile("arial.ttf")) {
        // If no external font, we'll use SFML's default font
        std::cout << "Using SFML default font as fallback" << std::endl;
    }
}

const sf::Font& MultiFontManager::getFontForLanguage(Language language) const {
    auto it = m_languageFonts.find(language);
    if (it != m_languageFonts.end()) {
        return *it->second;
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

    // Always include English as it's the fallback
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

// Enhanced LanguageManager Implementation
EnhancedLanguageManager& EnhancedLanguageManager::instance() {
    static EnhancedLanguageManager instance;
    return instance;
}

void EnhancedLanguageManager::setLanguage(Language lang) {
    // Check if the language is supported with proper fonts
    if (!MultiFontManager::instance().isLanguageSupported(lang)) {
        std::cout << "Warning: Language " << static_cast<int>(lang)
            << " may not display correctly due to missing fonts" << std::endl;
    }

    // Call parent implementation
    LanguageManager::setLanguage(lang);

    std::cout << "Language changed to: " << getLanguageDisplayName(lang) << std::endl;
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
    switch (lang) {
    case Language::ENGLISH:
        return "English";
    case Language::ARABIC:
        return "العربية"; // Arabic in Arabic script
    case Language::HEBREW:
        return "עברית"; // Hebrew in Hebrew script
    default:
        return "Unknown";
    }
}
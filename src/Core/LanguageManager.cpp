// src/Core/LanguageManager.cpp - IMPROVED VERSION
#include "LanguageManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

LanguageManager& LanguageManager::instance() {
    static LanguageManager instance;
    return instance;
}

void LanguageManager::setLanguage(Language lang) {
    m_currentLanguage = lang;
    if (m_texts[lang].empty()) {
        loadDefaultTexts();
    }
    std::cout << "Language set to: " << static_cast<int>(lang) << std::endl;
}

const std::string& LanguageManager::getText(const std::string& key) const {
    return getTextForLanguage(m_currentLanguage, key);
}

const std::string& LanguageManager::getTextForLanguage(Language lang, const std::string& key) const {
    auto langIt = m_texts.find(lang);
    if (langIt != m_texts.end()) {
        auto textIt = langIt->second.find(key);
        if (textIt != langIt->second.end()) {
            return textIt->second;
        }
    }

    // Fallback to English if not found
    if (lang != Language::ENGLISH) {
        return getTextForLanguage(Language::ENGLISH, key);
    }

    static const std::string empty = "";
    return empty;
}

void LanguageManager::loadLanguageFile(Language lang, const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cout << "Could not open language file: " << filePath << ", loading defaults" << std::endl;
        loadDefaultTexts();
        return;
    }
    //ads

    std::string line;
    while (std::getline(file, line)) {
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            m_texts[lang][key] = value;
        }
    }

    std::cout << "Loaded language file for language " << static_cast<int>(lang) << std::endl;
}

void LanguageManager::loadDefaultTexts() {
    std::cout << "Loading default texts for all languages..." << std::endl;

    // English texts
    m_texts[Language::ENGLISH]["settings_title"] = "Settings";
    m_texts[Language::ENGLISH]["settings_volume"] = "Volume";
    m_texts[Language::ENGLISH]["settings_language"] = "Language";
    m_texts[Language::ENGLISH]["settings_back"] = "Press ESC to go back";

    // Arabic texts (العربية)
    m_texts[Language::ARABIC]["settings_title"] = "الإعدادات";
    m_texts[Language::ARABIC]["settings_volume"] = "مستوى الصوت";
    m_texts[Language::ARABIC]["settings_language"] = "اللغة";
    m_texts[Language::ARABIC]["settings_back"] = "اضغط ESC للعودة";

    // Hebrew texts (עברית)
    m_texts[Language::HEBREW]["settings_title"] = "הגדרות";
    m_texts[Language::HEBREW]["settings_volume"] = "עוצמת קול";
    m_texts[Language::HEBREW]["settings_language"] = "שפה";
    m_texts[Language::HEBREW]["settings_back"] = "לחץ ESC לחזור";

    std::cout << "Default texts loaded for all languages" << std::endl;
}
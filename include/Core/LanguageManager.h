#pragma once

#include <unordered_map>
#include <string>
#include <memory>

enum class Language {
    ENGLISH,
    HEBREW,
    ARABIC
};

class LanguageManager {
public:
    static LanguageManager& instance();
    LanguageManager() = default;

    void setLanguage(Language lang);
    Language getCurrentLanguage() const { return m_currentLanguage; }

    const std::string& getText(const std::string& key) const;
    void loadLanguageFile(Language lang, const std::string& filePath);

private:
    Language m_currentLanguage = Language::ENGLISH;
    std::unordered_map<Language, std::unordered_map<std::string, std::string>> m_texts;

    void loadDefaultTexts();
    const std::string& getTextForLanguage(Language lang, const std::string& key) const;
};
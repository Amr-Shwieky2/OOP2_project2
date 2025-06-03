#include "LanguageManager.h"
#include <fstream>
#include <sstream>

LanguageManager& LanguageManager::instance() {
    static LanguageManager instance;
    return instance;
}

void LanguageManager::setLanguage(Language lang) {
    m_currentLanguage = lang;
    if (m_texts[lang].empty()) {
        loadDefaultTexts();
    }
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
        loadDefaultTexts();
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            m_texts[lang][key] = value;
        }
    }
}

void LanguageManager::loadDefaultTexts() {
    // English texts
    m_texts[Language::ENGLISH]["menu_start"] = "START GAME";
    m_texts[Language::ENGLISH]["menu_settings"] = "SETTINGS";
    m_texts[Language::ENGLISH]["menu_help"] = "HELP";
    m_texts[Language::ENGLISH]["menu_exit"] = "EXIT";
    m_texts[Language::ENGLISH]["settings_title"] = "Settings";
    m_texts[Language::ENGLISH]["settings_volume"] = "Volume";
    m_texts[Language::ENGLISH]["settings_language"] = "Language";
    m_texts[Language::ENGLISH]["settings_back"] = "Press ESC to go back";
    m_texts[Language::ENGLISH]["help_title"] = "Help";
    m_texts[Language::ENGLISH]["help_content"] = "How to play:\n\n- Use Arrow Keys to navigate menus\n- Press Enter to select\n- Use A/D keys to move in game\n- Press ESC to go back";
    m_texts[Language::ENGLISH]["language_english"] = "English";
    m_texts[Language::ENGLISH]["language_hebrew"] = "Hebrew";
    m_texts[Language::ENGLISH]["language_arabic"] = "Arabic";

    // Hebrew texts
    m_texts[Language::HEBREW]["menu_start"] = "התחל משחק";
    m_texts[Language::HEBREW]["menu_settings"] = "הגדרות";
    m_texts[Language::HEBREW]["menu_help"] = "עזרה";
    m_texts[Language::HEBREW]["menu_exit"] = "יציאה";
    m_texts[Language::HEBREW]["settings_title"] = "הגדרות";
    m_texts[Language::HEBREW]["settings_volume"] = "עוצמת קול";
    m_texts[Language::HEBREW]["settings_language"] = "שפה";
    m_texts[Language::HEBREW]["settings_back"] = "לחץ ESC כדי לחזור";
    m_texts[Language::HEBREW]["help_title"] = "עזרה";
    m_texts[Language::HEBREW]["help_content"] = "איך לשחק:\n\n- השתמש במקשי החצים לניווט\n- לחץ Enter לבחירה\n- השתמש במקשים A/D לתנועה במשחק\n- לחץ ESC לחזרה";
    m_texts[Language::HEBREW]["language_english"] = "אנגלית";
    m_texts[Language::HEBREW]["language_hebrew"] = "עברית";
    m_texts[Language::HEBREW]["language_arabic"] = "ערבית";

    // Arabic texts
    m_texts[Language::ARABIC]["menu_start"] = "ابدأ اللعبة";
    m_texts[Language::ARABIC]["menu_settings"] = "الإعدادات";
    m_texts[Language::ARABIC]["menu_help"] = "المساعدة";
    m_texts[Language::ARABIC]["menu_exit"] = "خروج";
    m_texts[Language::ARABIC]["settings_title"] = "الإعدادات";
    m_texts[Language::ARABIC]["settings_volume"] = "مستوى الصوت";
    m_texts[Language::ARABIC]["settings_language"] = "اللغة";
    m_texts[Language::ARABIC]["settings_back"] = "اضغط ESC للعودة";
    m_texts[Language::ARABIC]["help_title"] = "المساعدة";
    m_texts[Language::ARABIC]["help_content"] = "كيفية اللعب:\n\n- استخدم أسهم الاتجاه للتنقل\n- اضغط Enter للاختيار\n- استخدم مفاتيح A/D للحركة في اللعبة\n- اضغط ESC للعودة";
    m_texts[Language::ARABIC]["language_english"] = "الإنجليزية";
    m_texts[Language::ARABIC]["language_hebrew"] = "العبرية";
    m_texts[Language::ARABIC]["language_arabic"] = "العربية";
}
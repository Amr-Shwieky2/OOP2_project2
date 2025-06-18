#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

// Forward declarations
class VolumeControlPanel;
class LanguageControlPanel;

/**
 * Settings Event Handler - معالج أحداث شاشة الإعدادات
 * يدير جميع أحداث الماوس ولوحة المفاتيح في شاشة الإعدادات
 * مع إدارة آمنة للذاكرة ومعالجة الأخطاء
 */
class SettingsEventHandler {
public:
    SettingsEventHandler();
    ~SettingsEventHandler();  // explicit destructor

    // Component registration - تسجيل المكونات
    void setVolumePanel(std::shared_ptr<VolumeControlPanel> panel);
    void setLanguagePanel(std::shared_ptr<LanguageControlPanel> panel);

    // Event handling - معالجة الأحداث
    void handleEvents(sf::RenderWindow& window);

    // State queries - استعلامات الحالة
    bool shouldExitToMenu() const;
    void resetExitFlag();

    // Safe cleanup - تنظيف آمن
    void cleanup();

private:
    // Event handling methods - دوال معالجة الأحداث
    void handleWindowEvents(sf::RenderWindow& window, const sf::Event& event);
    void handleKeyboardEvents(const sf::Event& event);
    void handleMouseEvents(const sf::Event& event);

    // Specific event handlers - معالجات أحداث محددة
    void handleKeyPressEvents(const sf::Event& event);
    void handleMouseMoveEvents(const sf::Event& event);
    void handleMousePressEvents(const sf::Event& event);
    void handleMouseReleaseEvents(const sf::Event& event);

    // Key processing - معالجة المفاتيح
    void processEscapeKey();
    void processEnterKey();

    // Mouse event delegation - تفويض أحداث الماوس
    bool delegateMouseEventToComponents(const sf::Event& event);

    // Event validation - التحقق من صحة الأحداث
    bool isValidMouseEvent(const sf::Event& event) const;
    bool isValidKeyboardEvent(const sf::Event& event) const;

private:
    // Components - المكونات
    std::shared_ptr<VolumeControlPanel> m_volumePanel;
    std::shared_ptr<LanguageControlPanel> m_languagePanel;

    // State - الحالة
    bool m_shouldExitToMenu = false;
};
#include "SettingsEventHandler.h"
#include "VolumeControlPanel.h"
#include "LanguageControlPanel.h"
#include <iostream>

SettingsEventHandler::SettingsEventHandler() : m_shouldExitToMenu(false) {
    std::cout << "SettingsEventHandler created" << std::endl;
}

SettingsEventHandler::~SettingsEventHandler() {
    cleanup();
    std::cout << "SettingsEventHandler destroyed safely" << std::endl;
}

void SettingsEventHandler::cleanup() {
    try {
        // منع أي events جديدة
        m_shouldExitToMenu = true;

        // تنظيف المؤشرات بأمان
        if (m_volumePanel) {
            m_volumePanel.reset();
        }

        if (m_languagePanel) {
            m_languagePanel.reset();
        }

        std::cout << "SettingsEventHandler cleanup completed" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Error during cleanup: " << e.what() << std::endl;
    }
}

void SettingsEventHandler::setVolumePanel(std::shared_ptr<VolumeControlPanel> panel) {
    if (panel) {
        m_volumePanel = panel;
        std::cout << "Volume panel registered with event handler" << std::endl;
    }
}

void SettingsEventHandler::setLanguagePanel(std::shared_ptr<LanguageControlPanel> panel) {
    if (panel) {
        m_languagePanel = panel;
        std::cout << "Language panel registered with event handler" << std::endl;
    }
}

// ← إضافة هذه الدالة المهمة
void SettingsEventHandler::resetExitFlag() {
    m_shouldExitToMenu = false;
    std::cout << "Exit flag reset" << std::endl;
}

bool SettingsEventHandler::shouldExitToMenu() const {
    return m_shouldExitToMenu;
}

void SettingsEventHandler::handleEvents(sf::RenderWindow& window) {
    // تحقق من الحالة قبل معالجة الأحداث
    if (m_shouldExitToMenu) {
        return; // تجنب معالجة events أثناء الخروج
    }

    sf::Event event;
    while (window.pollEvent(event)) {
        try {
            // Handle different event types
            handleWindowEvents(window, event);
            handleKeyboardEvents(event);
            handleMouseEvents(event);
        }
        catch (const std::exception& e) {
            std::cout << "Error handling event: " << e.what() << std::endl;
            // في حالة خطأ حرج، اطلب الخروج
            m_shouldExitToMenu = true;
        }
        catch (...) {
            std::cout << "Unknown error handling event" << std::endl;
            m_shouldExitToMenu = true;
        }
    }

    // إضافة تجربة polling مباشر كحل بديل
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        if (!m_shouldExitToMenu) {
            std::cout << "Escape detected via polling - requesting exit to menu" << std::endl;
            m_shouldExitToMenu = true;
        }
    }
}

void SettingsEventHandler::handleWindowEvents(sf::RenderWindow& window, const sf::Event& event) {
    if (event.type == sf::Event::Closed) {
        std::cout << "Window close event received" << std::endl;
        window.close();
    }
}

void SettingsEventHandler::handleKeyboardEvents(const sf::Event& event) {
    if (!isValidKeyboardEvent(event)) return;

    // إضافة debug للكيبورد
    if (event.type == sf::Event::KeyPressed) {
        std::cout << "Key pressed detected: " << event.key.code << std::endl;
        handleKeyPressEvents(event);
    }
}

void SettingsEventHandler::handleKeyPressEvents(const sf::Event& event) {
    std::cout << "Processing key code: " << event.key.code << " (Escape=" << sf::Keyboard::Escape << ")" << std::endl;

    switch (event.key.code) {
    case sf::Keyboard::Escape:
        std::cout << "Escape case matched!" << std::endl;
        processEscapeKey();
        break;

    case sf::Keyboard::Enter:    // ← الإصلاح الأول
        processEnterKey();
        break;

    default:
        std::cout << "Unhandled key: " << event.key.code << std::endl;
        break;
    }
}

void SettingsEventHandler::handleMouseEvents(const sf::Event& event) {
    if (!isValidMouseEvent(event)) return;

    switch (event.type) {
    case sf::Event::MouseMoved:
        handleMouseMoveEvents(event);
        break;

    case sf::Event::MouseButtonPressed:
        handleMousePressEvents(event);
        break;

    case sf::Event::MouseButtonReleased:
        handleMouseReleaseEvents(event);
        break;

    default:
        break;
    }
}

void SettingsEventHandler::handleMouseMoveEvents(const sf::Event& event) {
    // Delegate to components
    delegateMouseEventToComponents(event);
}

void SettingsEventHandler::handleMousePressEvents(const sf::Event& event) {
    if (event.mouseButton.button == sf::Mouse::Left) {
        // Delegate to components
        bool handled = delegateMouseEventToComponents(event);

        if (handled) {
            std::cout << "Mouse press handled by component" << std::endl;
        }
    }
}

void SettingsEventHandler::handleMouseReleaseEvents(const sf::Event& event) {
    if (event.mouseButton.button == sf::Mouse::Left) {
        // Delegate to components
        delegateMouseEventToComponents(event);
    }
}

bool SettingsEventHandler::delegateMouseEventToComponents(const sf::Event& event) {
    // تحقق من صحة الحالة أولاً
    if (m_shouldExitToMenu) {
        return false; // تجنب معالجة events أثناء الخروج
    }

    bool handled = false;

    try {
        // تحقق من صحة المؤشرات قبل الاستخدام
        if (m_volumePanel && m_volumePanel.get() != nullptr) {
            handled |= m_volumePanel->handleMouseEvent(event);
        }

        if (m_languagePanel && m_languagePanel.get() != nullptr && !handled) {
            handled |= m_languagePanel->handleMouseEvent(event);
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error delegating mouse event: " << e.what() << std::endl;
        // في حالة خطأ، ننظف ونطلب الخروج
        cleanup();
        m_shouldExitToMenu = true;
    }

    return handled;
}

void SettingsEventHandler::processEscapeKey() {
    if (!m_shouldExitToMenu) {  // ← منع التكرار
        std::cout << "Escape key pressed - requesting exit to menu" << std::endl;
        std::cout << "Setting exit flag to true" << std::endl;
        m_shouldExitToMenu = true;
    }
    else {
        std::cout << "Escape already processed, ignoring" << std::endl;
    }
}

void SettingsEventHandler::processEnterKey() {
    // Could be used for confirming settings or other actions
    std::cout << "Enter key pressed in settings" << std::endl;

    // Save any pending changes
    try {
        if (m_volumePanel && m_volumePanel.get() != nullptr && m_volumePanel->hasChanged()) {
            m_volumePanel->saveSettings();
        }

        if (m_languagePanel && m_languagePanel.get() != nullptr && m_languagePanel->hasChanged()) {
            m_languagePanel->saveLanguagePreference();
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error saving settings on Enter: " << e.what() << std::endl;
    }
}

bool SettingsEventHandler::isValidMouseEvent(const sf::Event& event) const {
    switch (event.type) {
    case sf::Event::MouseMoved:
    case sf::Event::MouseButtonPressed:
    case sf::Event::MouseButtonReleased:
        return true;
    default:
        return false;
    }
}

bool SettingsEventHandler::isValidKeyboardEvent(const sf::Event& event) const {
    switch (event.type) {
    case sf::Event::KeyPressed:
    case sf::Event::KeyReleased:
        return true;
    default:
        return false;
    }
}
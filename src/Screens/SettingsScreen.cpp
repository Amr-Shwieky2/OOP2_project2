#include "../../include/Screens/SettingsScreen.h"
#include "VolumeControlPanel.h"
#include "LanguageControlPanel.h"
#include "SettingsEventHandler.h"
#include "../Core/LanguageManager.h"
#include <iostream>
#include <AppContext.h>

// في SettingsScreen constructor - تأكد من التهيئة الآمنة:

SettingsScreen::SettingsScreen() {
    try {
        std::cout << "Initializing SettingsScreen..." << std::endl;

        // تهيئة المتغيرات بـ nullptr أولاً
        m_volumePanel = nullptr;
        m_languagePanel = nullptr;
        m_eventHandler = nullptr;
        m_isInitialized = false;

        initializeResources();
        initializeComponents();

        // تحقق من نجاح التهيئة قبل wiring
        if (m_volumePanel && m_languagePanel) {
            wireComponents();
            m_isInitialized = true;
            std::cout << "SettingsScreen initialized successfully" << std::endl;
        }
        else {
            std::cout << "ERROR: Components not initialized properly!" << std::endl;
            std::cout << "VolumePanel: " << (m_volumePanel ? "OK" : "NULL") << std::endl;
            std::cout << "LanguagePanel: " << (m_languagePanel ? "OK" : "NULL") << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cout << "CRITICAL ERROR in SettingsScreen constructor: " << e.what() << std::endl;
        handleInitializationError("SettingsScreen: " + std::string(e.what()));
    }
}

SettingsScreen::~SettingsScreen() {
    try {
        std::cout << "Destroying SettingsScreen..." << std::endl;
        cleanup();
        std::cout << "SettingsScreen destroyed safely" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Error destroying SettingsScreen: " << e.what() << std::endl;
    }
}

void SettingsScreen::handleEvents(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        // التعامل مع إغلاق النافذة
        if (event.type == sf::Event::Closed) {
            window.close();
            return;
        }

        // التعامل مع ESC
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            std::cout << "SettingsScreen: ESC pressed, returning to main menu..." << std::endl;
            AppContext::instance().screenManager().changeScreen(ScreenType::MENU);
            return;
        }

        // التحقق الآمن من الـ components قبل استخدامها
        bool eventHandled = false;

        // التحقق من Language Panel
        if (m_languagePanel != nullptr) {
            try {
                eventHandled = m_languagePanel->handleMouseEvent(event);
                if (eventHandled) {
                    std::cout << "Event handled by LanguageControlPanel" << std::endl;
                }
            }
            catch (const std::exception& e) {
                std::cout << "Error in LanguageControlPanel::handleMouseEvent: " << e.what() << std::endl;
                eventHandled = false;
            }
        }

        // التحقق من Volume Panel
        if (m_volumePanel != nullptr && !eventHandled) {
            try {
                eventHandled = m_volumePanel->handleMouseEvent(event);
                if (eventHandled) {
                    std::cout << "Event handled by VolumeControlPanel" << std::endl;
                }
            }
            catch (const std::exception& e) {
                std::cout << "Error in VolumeControlPanel::handleMouseEvent: " << e.what() << std::endl;
                eventHandled = false;
            }
        }
    }
}

void SettingsScreen::update(float deltaTime) {
    try {
        if (!m_isInitialized) {
            return;
        }

        updateAnimation(deltaTime);
        updateComponents(deltaTime);
    }
    catch (const std::exception& e) {
        handleUpdateError("Update", e);
    }
}

void SettingsScreen::render(sf::RenderWindow& window) {
    try {
        if (!m_isInitialized) {
            return;
        }

        renderBackground(window);
        renderScreenTexts(window);
        renderComponents(window);
        renderAnimationEffects(window);
    }
    catch (const std::exception& e) {
        handleRenderError("Render", e);
    }
}

// Additional methods for screen management
bool SettingsScreen::shouldReturnToMenu() const {
    return m_shouldReturnToMenu;
}

void SettingsScreen::onEnter() {
    try {
        std::cout << "Entering Settings Screen" << std::endl;

        // Reset animation
        m_animationTime = 0.0f;

        // Reset exit flag
        m_shouldReturnToMenu = false;
        if (m_eventHandler) {
            m_eventHandler->resetExitFlag();
        }

        std::cout << "Settings Screen entered successfully" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Error entering settings screen: " << e.what() << std::endl;
    }
}

void SettingsScreen::onExit() {
    try {
        std::cout << "Exiting Settings Screen" << std::endl;
        // Auto-save any changes here if needed
        std::cout << "Settings Screen exited successfully" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Error exiting settings screen: " << e.what() << std::endl;
    }
}

// Initialization methods
void SettingsScreen::initializeResources() {
    try {
        // Load shared resources
        if (!loadFont()) {
            std::cout << "Warning: Using default font for SettingsScreen" << std::endl;
        }

        initializeBackground();
        initializeTexts();

        std::cout << "Resources initialized successfully" << std::endl;
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Resource initialization failed: " + std::string(e.what()));
    }
}

void SettingsScreen::initializeComponents() {
    try {
        std::cout << "Creating components..." << std::endl;

        // Create volume control panel بحذر
        try {
            m_volumePanel = std::make_shared<VolumeControlPanel>(m_font);
            if (m_volumePanel) {
                std::cout << "Volume control panel created successfully" << std::endl;
            }
            else {
                std::cout << "ERROR: Volume control panel is null after creation!" << std::endl;
            }
        }
        catch (const std::exception& e) {
            std::cout << "ERROR creating VolumeControlPanel: " << e.what() << std::endl;
            m_volumePanel = nullptr;
        }
        try {
            m_languagePanel = std::make_shared<LanguageControlPanel>(m_font);
            if (m_languagePanel) {
                std::cout << "Language control panel created successfully" << std::endl;
            }
            else {
                std::cout << "ERROR: Language control panel is null after creation!" << std::endl;
            }
        }
        catch (const std::exception& e) {
            std::cout << "ERROR creating LanguageControlPanel: " << e.what() << std::endl;
            m_languagePanel = nullptr;
        }

        try {
            m_eventHandler = std::make_unique<SettingsEventHandler>();
            if (m_eventHandler) {
                std::cout << "Settings event handler created successfully" << std::endl;
            }
        }
        catch (const std::exception& e) {
            std::cout << "ERROR creating SettingsEventHandler: " << e.what() << std::endl;
            m_eventHandler = nullptr;
        }

    }
    catch (const std::exception& e) {
        std::cout << "CRITICAL ERROR in initializeComponents: " << e.what() << std::endl;
        throw std::runtime_error("Component initialization failed: " + std::string(e.what()));
    }
}

void SettingsScreen::initializeBackground() {
    try {
        if (loadBackgroundTexture()) {
            m_backgroundSprite.setTexture(m_backgroundTexture);

            // Scale to fit window
            sf::Vector2u textureSize = m_backgroundTexture.getSize();
            if (textureSize.x > 0 && textureSize.y > 0) {
                sf::Vector2f targetSize(1400.0f, 800.0f);
                float scaleX = targetSize.x / textureSize.x;
                float scaleY = targetSize.y / textureSize.y;
                m_backgroundSprite.setScale(scaleX, scaleY);
            }
            std::cout << "Background texture loaded and scaled" << std::endl;
        }
        else {
            setupFallbackBackground();
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error loading background: " << e.what() << std::endl;
        setupFallbackBackground();
    }
}

void SettingsScreen::initializeTexts() {
    auto& lang = LanguageManager::instance();

    // Setup title text
    m_titleText.setFont(m_font);
    m_titleText.setString(lang.getText("settings_title"));
    m_titleText.setCharacterSize(48);
    m_titleText.setFillColor(sf::Color(139, 69, 19));
    m_titleText.setPosition(550, 50);
    m_titleText.setStyle(sf::Text::Bold);

    // Setup back instruction text
    m_backInstructionText.setFont(m_font);
    m_backInstructionText.setString(lang.getText("settings_back"));
    m_backInstructionText.setCharacterSize(20);
    m_backInstructionText.setFillColor(sf::Color(139, 69, 19));
    m_backInstructionText.setPosition(500, 650);
    m_backInstructionText.setStyle(sf::Text::Italic);

    std::cout << "Screen texts initialized" << std::endl;
}

void SettingsScreen::wireComponents() {
    try {
        // Connect event handler to components using shared_ptr (safe)
        if (m_eventHandler) {
            m_eventHandler->setVolumePanel(m_volumePanel);
            m_eventHandler->setLanguagePanel(m_languagePanel);
            std::cout << "Components wired to event handler" << std::endl;
        }
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Component wiring failed: " + std::string(e.what()));
    }
}

// Update methods
void SettingsScreen::updateAnimation(float deltaTime) {
    m_animationTime += deltaTime;

    // Animate title glow effect
    float glowIntensity = 0.7f + 0.3f * sin(m_animationTime * 2.0f);
    sf::Color titleColor = sf::Color(101, 67, 33); // Dark Brown base color
    titleColor.a = static_cast<sf::Uint8>(255 * glowIntensity);
    m_titleText.setFillColor(titleColor);
}

void SettingsScreen::updateComponents(float deltaTime) {
    try {
        if (m_volumePanel) {
            m_volumePanel->update(deltaTime);
        }

        if (m_languagePanel) {
            m_languagePanel->update(deltaTime);
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error updating components: " << e.what() << std::endl;
    }
}

void SettingsScreen::checkForScreenTransition() {
    if (m_eventHandler && m_eventHandler->shouldExitToMenu()) {
        std::cout << "Screen transition requested - exit to menu" << std::endl;

        // تعيين flag مرة واحدة فقط
        if (!m_shouldReturnToMenu) {
            m_shouldReturnToMenu = true;
            std::cout << "Set flag to return to menu - will be handled by screen manager" << std::endl;
        }

        // إعادة تعيين flag في EventHandler
        m_eventHandler->resetExitFlag();
    }
}

// Render methods with enhanced visuals
void SettingsScreen::renderBackground(sf::RenderWindow& window) {
    if (m_backgroundTexture.getSize().x > 0) {
        window.draw(m_backgroundSprite);
    }
}

void SettingsScreen::renderScreenTexts(sf::RenderWindow& window) {
    // Draw title with shadow effect
    sf::Text titleShadow = m_titleText;
    titleShadow.setFillColor(sf::Color(50, 33, 17, 120)); // Dark brown shadow
    titleShadow.setPosition(m_titleText.getPosition().x + 3, m_titleText.getPosition().y + 3);
    window.draw(titleShadow);
    window.draw(m_titleText);

    // Draw back instruction
    window.draw(m_backInstructionText);
}

void SettingsScreen::renderComponents(sf::RenderWindow& window) {
    if (m_volumePanel) {
        m_volumePanel->render(window);
    }

    if (m_languagePanel) {
        m_languagePanel->render(window);
    }
}

void SettingsScreen::renderAnimationEffects(sf::RenderWindow& window) {
    // Additional visual effects can be added here
    // For example: particle effects, transitions, etc.
}

// Resource management
bool SettingsScreen::loadFont() {
    try {
        // Try to load the preferred font
        if (m_font.loadFromFile("arial.ttf")) {
            return true;
        }

        std::cout << "Could not load external font, using SFML default" << std::endl;
        return false;
    }
    catch (const std::exception& e) {
        std::cout << "Exception loading font: " << e.what() << std::endl;
        return false;
    }
}

bool SettingsScreen::loadBackgroundTexture() {
    try {
        return m_backgroundTexture.loadFromFile("SettingsScreen.png");
    }
    catch (const std::exception& e) {
        std::cout << "Exception loading background: " << e.what() << std::endl;
        return false;
    }
}

void SettingsScreen::setupFallbackBackground() {
    std::cout << "Setting up fallback background" << std::endl;

    // Create a simple gradient background
    const unsigned int width = 1400;
    const unsigned int height = 800;

    m_backgroundTexture.create(width, height);
    sf::Uint8* pixels = new sf::Uint8[width * height * 4];

    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            unsigned int index = (y * width + x) * 4;
            float gradient = static_cast<float>(y) / height;

            pixels[index] = static_cast<sf::Uint8>(50 + 100 * gradient);     // R
            pixels[index + 1] = static_cast<sf::Uint8>(30 + 80 * gradient);  // G
            pixels[index + 2] = static_cast<sf::Uint8>(80 + 120 * gradient); // B
            pixels[index + 3] = 255; // A
        }
    }

    m_backgroundTexture.update(pixels);
    m_backgroundSprite.setTexture(m_backgroundTexture);
    delete[] pixels;
}

void SettingsScreen::cleanup() {
    try {
        // تنظيف EventHandler أولاً
        if (m_eventHandler) {
            m_eventHandler->cleanup();
            m_eventHandler.reset();
        }

        // ثم باقي المكونات
        if (m_volumePanel) {
            m_volumePanel.reset();
        }

        if (m_languagePanel) {
            m_languagePanel.reset();
        }

        std::cout << "SettingsScreen cleanup completed" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Error during cleanup: " << e.what() << std::endl;
    }
}

// Error handling
void SettingsScreen::handleInitializationError(const std::string& component) {
    std::cout << "Initialization error in " << component << std::endl;
    m_isInitialized = false;
}

void SettingsScreen::handleUpdateError(const std::string& component, const std::exception& e) {
    std::cout << "Update error in " << component << ": " << e.what() << std::endl;
}

void SettingsScreen::handleRenderError(const std::string& component, const std::exception& e) {
    std::cout << "Render error in " << component << ": " << e.what() << std::endl;
}
#pragma once
#include "IScreen.h"
#include <SFML/Graphics.hpp>
#include <memory>

// Forward declarations
class VolumeControlPanel;
class LanguageControlPanel;
class SettingsEventHandler;

/**
 * Settings Screen - Coordinator for settings components
 * Single Responsibility: Screen lifecycle and component coordination
 *
 * This class follows the Single Responsibility Principle by:
 * - Only handling screen lifecycle (IScreen interface)
 * - Coordinating between specialized components
 * - Managing shared resources (font, background)
 * - Enhanced visual effects and animations
 * - Safe memory management with smart pointers
 * - Safe screen transitions without self-destruction
 */
class SettingsScreen : public IScreen {
public:
    SettingsScreen();
    ~SettingsScreen();

    // IScreen interface - متطابق مع Interface الأصلي
    void handleEvents(sf::RenderWindow& window) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

    // Additional methods for screen management
    bool shouldReturnToMenu() const;
    void resetMenuFlag() { m_shouldReturnToMenu = false; }  // ← إضافة هذا
    void onEnter();  // للاستدعاء من Screen Manager
    void onExit();   // للاستدعاء من Screen Manager

private:
    // Specialized components - using shared_ptr for safe sharing
    std::shared_ptr<VolumeControlPanel> m_volumePanel;
    std::shared_ptr<LanguageControlPanel> m_languagePanel;
    std::unique_ptr<SettingsEventHandler> m_eventHandler;

    // Shared screen resources
    sf::Font m_font;
    sf::Texture m_backgroundTexture;
    sf::Sprite m_backgroundSprite;

    // Screen-level UI elements
    sf::Text m_titleText;
    sf::Text m_backInstructionText;

    // Animation state
    float m_animationTime = 0.0f;

    // State management
    bool m_isInitialized = false;
    bool m_shouldReturnToMenu = false;

    // Initialization methods - each with single purpose
    void initializeResources();
    void initializeComponents();
    void initializeBackground();
    void initializeTexts();
    void wireComponents();

    // Update methods - separated by concern
    void updateAnimation(float deltaTime);
    void updateComponents(float deltaTime);
    void checkForScreenTransition();

    // Render methods - separated by layer
    void renderBackground(sf::RenderWindow& window);
    void renderComponents(sf::RenderWindow& window);
    void renderScreenTexts(sf::RenderWindow& window);
    void renderAnimationEffects(sf::RenderWindow& window);

    // Resource management
    bool loadFont();
    bool loadBackgroundTexture();
    void setupFallbackBackground();
    void cleanup();

    // Error handling
    void handleInitializationError(const std::string& component);
    void handleUpdateError(const std::string& component, const std::exception& e);
    void handleRenderError(const std::string& component, const std::exception& e);
};
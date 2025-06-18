#pragma once
#include "../Core/IScreen.h"
#include "SettingsResourceManager.h"
#include "SettingsUIRenderer.h" 
#include "../Commands/SettingsCommandHandler.h"
#include "../UI/VolumeControlPanel.h"
#include "../UI/LanguageControlPanel.h"
#include <SFML/Graphics.hpp>
#include <memory>

class SettingsScreen : public IScreen {
public:
    /**
     * @brief Constructor implementing Composition Pattern
     * Creates specialized components instead of handling everything internally
     */
    SettingsScreen();

    /**
     * @brief Destructor with automatic cleanup (RAII)
     * Smart pointers handle cleanup automatically
     */
    ~SettingsScreen() = default;

    /**
     * @brief IScreen interface implementation with delegation
     * Each method delegates to appropriate specialized component
     */
    void handleEvents(sf::RenderWindow& window) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

    /**
     * @brief Screen lifecycle management
     * Clean entry/exit points for state management
     */
    void onEnter();
    void onExit();

    /**
     * @brief Configuration methods for flexibility
     */
    void enableAutoSave(bool enable);
    void setAnimationSpeed(float speed);

    /**
     * @brief Status queries for external systems
     */
    bool isInitialized() const { return m_isInitialized; }
    bool hasUnsavedChanges() const;

private:
    /**
     * @brief Specialized components implementing Composition Pattern
     * Each component handles a specific aspect of the screen
     */
    std::unique_ptr<SettingsResourceManager> m_resourceManager;
    std::unique_ptr<SettingsUIRenderer> m_uiRenderer;
    std::unique_ptr<SettingsCommandHandler> m_commandHandler;

    /**
     * @brief UI panels using shared_ptr for safe memory management
     * Shared between command handler (for auto-save) and main screen (for rendering)
     */
    std::shared_ptr<VolumeControlPanel> m_volumePanel;
    std::shared_ptr<LanguageControlPanel> m_languagePanel;

    /**
     * @brief State management
     */
    bool m_isInitialized = false;

    /**
     * @brief Initialization methods - each with single purpose
     * Demonstrates decomposition of complex initialization
     */
    void initializeComponents();
    void setupUIComponents();
    void wireComponentsTogether();
    void configureInitialSettings();

    /**
     * @brief Event delegation to specialized handlers
     * Demonstrates Chain of Responsibility pattern
     */
    bool delegateMouseEvents(const sf::Event& event);
    bool delegateKeyboardEvents(const sf::Event& event);

    /**
     * @brief Error handling with specific error types
     */
    void handleInitializationError(const std::string& component);
    void handleRuntimeError(const std::string& operation, const std::exception& e);

    bool validateComponentsIntegrity() const;
};
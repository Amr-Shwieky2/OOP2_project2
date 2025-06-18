#include "../../include/Screens/SettingsScreen.h"
#include "../Application/AppContext.h"
#include <iostream>

SettingsScreen::SettingsScreen() {
    try {
        initializeComponents();
        setupUIComponents();
        wireComponentsTogether();
        configureInitialSettings();

        if (validateComponentsIntegrity()) {
            m_isInitialized = true;
            std::cout << "SettingsScreen initialized successfully" << std::endl;
        }
        else {
            throw std::runtime_error("Component integrity validation failed");
        }

    }
    catch (const std::exception& e) {
        handleInitializationError(e.what());
    }
}

void SettingsScreen::handleEvents(sf::RenderWindow& window) {
    if (!m_isInitialized) return;

    sf::Event event;
    while (window.pollEvent(event)) {
        // Handle window close
        if (event.type == sf::Event::Closed) {
            window.close();
            return;
        }

        // Handle keyboard input using Command Pattern
        if (event.type == sf::Event::KeyPressed) {
            if (m_commandHandler && m_commandHandler->handleKeyboardInput(event)) {
                return; // Command was handled
            }
        }

        // Handle mouse events for UI components
        delegateMouseEvents(event);
    }
}

/**
 * @brief Update method with component coordination
 * Each component handles its own updates independently
 */
void SettingsScreen::update(float deltaTime) {
    if (!m_isInitialized) return;

    try {
        // Update animations and visual effects
        if (m_uiRenderer) {
            m_uiRenderer->updateAnimation(deltaTime);
        }

        // Update interactive components
        if (m_volumePanel) m_volumePanel->update(deltaTime);
        if (m_languagePanel) m_languagePanel->update(deltaTime);

    }
    catch (const std::exception& e) {
        handleRuntimeError("Update", e);
    }
}

/**
 * @brief Render method with rendering pipeline separation
 * Demonstrates clear separation of rendering responsibilities
 */
void SettingsScreen::render(sf::RenderWindow& window) {
    if (!m_isInitialized) return;

    try {
        // Render background
        if (m_uiRenderer && m_resourceManager) {
            m_uiRenderer->renderBackground(window, m_resourceManager->getBackgroundSprite());
        }

        // Render texts with effects
        if (m_uiRenderer) {
            m_uiRenderer->renderTexts(window);
        }

        // Render interactive components
        if (m_volumePanel) m_volumePanel->render(window);
        if (m_languagePanel) m_languagePanel->render(window);

        // Render additional effects
        if (m_uiRenderer) {
            m_uiRenderer->renderAnimationEffects(window);
        }

    }
    catch (const std::exception& e) {
        handleRuntimeError("Render", e);
    }
}

/**
 * @brief Initialize specialized components with Factory-like pattern
 */
void SettingsScreen::initializeComponents() {
    // Create Resource Manager (needed by other components)
    m_resourceManager = std::make_unique<SettingsResourceManager>();
    if (!m_resourceManager->initializeResources()) {
        throw std::runtime_error("Failed to initialize SettingsResourceManager");
    }

    // Create UI Renderer with dependency injection
    m_uiRenderer = std::make_unique<SettingsUIRenderer>(m_resourceManager->getFont());
    m_uiRenderer->initializeTexts();

    // Create Command Handler
    m_commandHandler = std::make_unique<SettingsCommandHandler>();
}

/**
 * @brief Setup UI Components with Factory Pattern
 */
void SettingsScreen::setupUIComponents() {
    try {
        // Create volume control with dependency injection
        m_volumePanel = std::make_shared<VolumeControlPanel>(m_resourceManager->getFont());
        if (!m_volumePanel) {
            throw std::runtime_error("Failed to create VolumeControlPanel");
        }

        // Create language control with dependency injection
        m_languagePanel = std::make_shared<LanguageControlPanel>(m_resourceManager->getFont());
        if (!m_languagePanel) {
            throw std::runtime_error("Failed to create LanguageControlPanel");
        }

    }
    catch (const std::exception& e) {
        throw std::runtime_error("UI Component setup failed: " + std::string(e.what()));
    }
}

/**
 * @brief Wire components using Dependency Injection
 */
void SettingsScreen::wireComponentsTogether() {
    // Inject UI panels into Command Handler for auto-save
    if (m_commandHandler) {
        m_commandHandler->setVolumePanel(m_volumePanel);
        m_commandHandler->setLanguagePanel(m_languagePanel);
    }
}

/**
 * @brief Configure initial settings for all components
 */
void SettingsScreen::configureInitialSettings() {
    // Configure UI effects
    if (m_uiRenderer) {
        m_uiRenderer->enableGlowEffect(true);
        m_uiRenderer->enableShadowEffect(true);
        m_uiRenderer->setAnimationSpeed(1.0f);
    }

    // Configure auto-save
    if (m_commandHandler) {
        m_commandHandler->enableAutoSave(true);
        m_commandHandler->setAutoSaveDelay(0.5f);
    }
}

/**
 * @brief Delegate keyboard events to Command Handler
 */
bool SettingsScreen::delegateKeyboardEvents(const sf::Event& event) {
    return m_commandHandler ? m_commandHandler->handleKeyboardInput(event) : false;
}

/**
 * @brief Delegate mouse events to UI components with priority order
 */
bool SettingsScreen::delegateMouseEvents(const sf::Event& event) {
    bool handled = false;

    // Try language panel first
    if (m_languagePanel && !handled) {
        try {
            handled = m_languagePanel->handleMouseEvent(event);
        }
        catch (const std::exception& e) {
            std::cout << "Error in LanguageControlPanel: " << e.what() << std::endl;
        }
    }

    // Try volume panel if not handled
    if (m_volumePanel && !handled) {
        try {
            handled = m_volumePanel->handleMouseEvent(event);
        }
        catch (const std::exception& e) {
            std::cout << "Error in VolumeControlPanel: " << e.what() << std::endl;
        }
    }

    return handled;
}

/**
 * @brief Screen lifecycle methods
 */
void SettingsScreen::onEnter() {
    // Reset animation speed on enter
    if (m_uiRenderer) {
        m_uiRenderer->setAnimationSpeed(1.0f);
    }
}

void SettingsScreen::onExit() {
    // Auto-save handled by Command Pattern
}

/**
 * @brief Configuration methods
 */
bool SettingsScreen::hasUnsavedChanges() const {
    bool volumeChanged = m_volumePanel && m_volumePanel->hasChanged();
    bool languageChanged = m_languagePanel && m_languagePanel->hasChanged();
    return volumeChanged || languageChanged;
}

void SettingsScreen::enableAutoSave(bool enable) {
    if (m_commandHandler) {
        m_commandHandler->enableAutoSave(enable);
    }
}

void SettingsScreen::setAnimationSpeed(float speed) {
    if (m_uiRenderer) {
        m_uiRenderer->setAnimationSpeed(speed);
    }
}

/**
 * @brief Validation and error handling
 */
bool SettingsScreen::validateComponentsIntegrity() const {
    return m_resourceManager && m_resourceManager->isInitialized() &&
        m_uiRenderer && m_commandHandler &&
        m_volumePanel && m_languagePanel;
}

void SettingsScreen::handleInitializationError(const std::string& component) {
    std::cout << "Initialization error: " << component << std::endl;
    m_isInitialized = false;
}

void SettingsScreen::handleRuntimeError(const std::string& operation, const std::exception& e) {
    std::cout << "Runtime error in " << operation << ": " << e.what() << std::endl;
}
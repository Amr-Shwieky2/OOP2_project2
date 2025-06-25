#include "../Application/AppContext.h"
#include <iostream>
#include "../../include/Screens/SettingsScreen.h"

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

        // Handle keyboard input using SAFE Command Pattern
        if (event.type == sf::Event::KeyPressed) {
            if (m_commandHandler) {
                bool shouldExit = m_commandHandler->handleKeyboardInput(event);
                if (shouldExit) {
                    // Exit to menu screen safely
                    std::cout << "SettingsScreen: Exiting to menu screen" << std::endl;
                    AppContext::instance().screenManager().changeScreen(ScreenType::MENU);
                    return;
                }
            }
        }

        // Handle mouse events for UI components
        delegateMouseEvents(event);
    }
}

void SettingsScreen::update(float deltaTime) {
    if (!m_isInitialized) return;

    try {
        // Update animations and visual effects
        if (m_uiRenderer) {
            m_uiRenderer->updateAnimation(deltaTime);
        }

        // Update interactive components
        if (m_volumePanel) m_volumePanel->update(deltaTime);

    }
    catch (const std::exception& e) {
        handleRuntimeError("Update", e);
    }
}

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

        // Render additional effects
        if (m_uiRenderer) {
            m_uiRenderer->renderAnimationEffects(window);
        }

    }
    catch (const std::exception& e) {
        handleRuntimeError("Render", e);
    }
}

void SettingsScreen::initializeComponents() {
    // Create Resource Manager (needed by other components)
    m_resourceManager = std::make_unique<SettingsResourceManager>();
    if (!m_resourceManager->initializeResources()) {
        throw std::runtime_error("Failed to initialize SettingsResourceManager");
    }

    // Create UI Renderer with dependency injection
    m_uiRenderer = std::make_unique<SettingsUIRenderer>(m_resourceManager->getFont());
    m_uiRenderer->initializeTexts();

    // Create SAFE Command Handler (no callbacks)
    m_commandHandler = std::make_unique<SettingsCommandHandler>();
}

void SettingsScreen::setupUIComponents() {
    try {
        // Create volume control with dependency injection
        m_volumePanel = std::make_shared<VolumeControlPanel>(m_resourceManager->getFont());
        if (!m_volumePanel) {
            throw std::runtime_error("Failed to create VolumeControlPanel");
        }

    }
    catch (const std::exception& e) {
        throw std::runtime_error("UI Component setup failed: " + std::string(e.what()));
    }
}

void SettingsScreen::wireComponentsTogether() {
    // Inject UI panels into Command Handler for auto-save
    if (m_commandHandler) {
        m_commandHandler->setVolumePanel(m_volumePanel);
    }
}

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

bool SettingsScreen::delegateMouseEvents(const sf::Event& event) {
    bool handled = false;

    // Try volume panel
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

void SettingsScreen::onEnter() {
    if (m_uiRenderer) {
        m_uiRenderer->setAnimationSpeed(1.0f);
    }
}

void SettingsScreen::onExit() {
    // Auto-save handled by Command Pattern
}

bool SettingsScreen::hasUnsavedChanges() const {
    bool volumeChanged = m_volumePanel && m_volumePanel->hasChanged();
    return volumeChanged;
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

bool SettingsScreen::validateComponentsIntegrity() const {
    return m_resourceManager && m_resourceManager->isInitialized() &&
        m_uiRenderer && m_commandHandler && m_volumePanel;
}

void SettingsScreen::handleInitializationError(const std::string& component) {
    std::cout << "Initialization error: " << component << std::endl;
    m_isInitialized = false;
}

void SettingsScreen::handleRuntimeError(const std::string& operation, const std::exception& e) {
    std::cout << "Runtime error in " << operation << ": " << e.what() << std::endl;
}
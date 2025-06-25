#pragma once
#include "../Core/IScreen.h"
#include "SettingsResourceManager.h"
#include "SettingsUIRenderer.h" 
#include "../Commands/SettingsCommandHandler.h"
#include "../UI/VolumeControlPanel.h"
#include <SFML/Graphics.hpp>
#include <memory>

class SettingsScreen : public IScreen {
public:
    SettingsScreen();
    ~SettingsScreen() = default;

    void handleEvents(sf::RenderWindow& window) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

    void onEnter();
    void onExit();
    void enableAutoSave(bool enable);
    void setAnimationSpeed(float speed);
    bool isInitialized() const { return m_isInitialized; }
    bool hasUnsavedChanges() const;

private:
    std::unique_ptr<SettingsResourceManager> m_resourceManager;
    std::unique_ptr<SettingsUIRenderer> m_uiRenderer;
    std::unique_ptr<SettingsCommandHandler> m_commandHandler;
    std::shared_ptr<VolumeControlPanel> m_volumePanel;

    bool m_isInitialized = false;

    void initializeComponents();
    void setupUIComponents();
    void wireComponentsTogether();
    void configureInitialSettings();
    bool delegateMouseEvents(const sf::Event& event);
    bool delegateKeyboardEvents(const sf::Event& event);
    void handleInitializationError(const std::string& component);
    void handleRuntimeError(const std::string& operation, const std::exception& e);
    bool validateComponentsIntegrity() const;
};
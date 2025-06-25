// File: include/Screens/SettingsCommandHandler.h
#pragma once
#include <SFML/Graphics.hpp>
#include "../Application/AppContext.h"
#include "../Commands/EscapeKeyCommand.h"
#include <memory>

// Forward declarations to avoid circular dependencies
class VolumeControlPanel;

class SettingsCommandHandler {
public:
    SettingsCommandHandler();
    ~SettingsCommandHandler() = default;

    /**
     * @brief Main input processing entry point
     * @param event Keyboard event to process
     * @return true if event was handled, false if ignored
     *
     * This method demonstrates the Chain of Responsibility pattern
     */
    bool handleKeyboardInput(const sf::Event& event);

    /**
     * @brief Individual command executors - Command Pattern implementation
     * Each method represents a specific command that can be executed
     */
    void executeEscapeCommand();
    bool executeUndo();
    bool executeRedo();
    void printCommandHistory();

    /**
     * @brief Dependency injection for UI panels
     * @param panel Shared pointer to volume control panel
     *
     * Uses weak_ptr to avoid circular dependencies and memory leaks
     */
    void setVolumePanel(std::shared_ptr<VolumeControlPanel> panel);

    /**
     * @brief Advanced features for demonstrating sophisticated design
     */
    void enableAutoSave(bool enable) { m_autoSaveEnabled = enable; }
    void setAutoSaveDelay(float seconds) { m_autoSaveDelay = seconds; }
    bool isAutoSaveEnabled() const { return m_autoSaveEnabled; }

private:
    // Panel references using weak_ptr for safe memory management
    std::weak_ptr<VolumeControlPanel> m_volumePanel;

    // Auto-save configuration
    bool m_autoSaveEnabled = true;
    float m_autoSaveDelay = 0.5f; // seconds

    /**
     * @brief Individual key handlers implementing Chain of Responsibility
     * Each handler processes specific input and returns success/failure
     */
    bool handleEscapeKey();
    bool handleUndoKey();
    bool handleRedoKey();
    bool handleHistoryKey();

    /**
     * @brief Auto-save system implementation
     * Demonstrates encapsulation of related functionality
     */
    void saveSettingsBeforeExit();
    bool shouldSaveVolumeSettings();
    void performAutoSave();

    /**
     * @brief Input validation and error handling
     */
    bool isValidKeyboardEvent(const sf::Event& event) const;
    bool isModifierKeyPressed(const sf::Event& event) const;

    /**
     * @brief Debug and logging utilities
     */
    void logCommandExecution(const std::string& commandName, bool success);
    void logInputEvent(const sf::Event& event);
    void logAutoSaveAction(const std::string& action);

    /**
     * @brief Command execution helpers
     */
    std::unique_ptr<EscapeKeyCommand> createEscapeCommand();
    void executeCommandSafely(std::unique_ptr<ICommand> command);
};
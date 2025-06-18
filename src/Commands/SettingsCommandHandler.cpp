#include "../../include/Commands/SettingsCommandHandler.h"
#include "../UI/VolumeControlPanel.h"
#include "../UI/LanguageControlPanel.h"
#include <iostream>

SettingsCommandHandler::SettingsCommandHandler() {
    std::cout << "Creating SettingsCommandHandler implementing Command Pattern..." << std::endl;
    std::cout << "Auto-save enabled: " << (m_autoSaveEnabled ? "Yes" : "No") << std::endl;
    std::cout << "Auto-save delay: " << m_autoSaveDelay << " seconds" << std::endl;
}

bool SettingsCommandHandler::handleKeyboardInput(const sf::Event& event) {
    // Validate input
    if (event.type != sf::Event::KeyPressed) {
        return false;
    }

    std::cout << "Processing key: " << event.key.code << std::endl;

    try {
        switch (event.key.code) {
        case sf::Keyboard::Escape:
            return handleEscapeKey();

        case sf::Keyboard::U:
            return executeUndo();

        case sf::Keyboard::R:
            return executeRedo();

        default:
            return false; // Key not handled
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error handling key: " << e.what() << std::endl;
        return false;
    }
}

bool SettingsCommandHandler::handleEscapeKey() {
    std::cout << "Handling Escape key..." << std::endl;

    try {
        // Auto-save if enabled
        if (m_autoSaveEnabled) {
            saveSettingsBeforeExit();
        }

        // Create and execute escape command
        auto escapeCommand = std::make_unique<EscapeKeyCommand>(
            ScreenType::SETTINGS,
            ScreenType::MENU
        );

        AppContext::instance().commandInvoker().execute(std::move(escapeCommand));

        std::cout << "Escape command executed" << std::endl;
        return true;

    }
    catch (const std::exception& e) {
        std::cout << "Error executing escape: " << e.what() << std::endl;
        return false;
    }
}
bool SettingsCommandHandler::handleUndoKey() {
    std::cout << "Processing Undo command for screen navigation..." << std::endl;

    bool success = executeUndo();
    logCommandExecution("UndoCommand", success);

    if (success) {
        std::cout << "Undo executed successfully - returned to previous screen!" << std::endl;
    }
    else {
        std::cout << "No actions available to undo" << std::endl;
    }

    return success;
}

bool SettingsCommandHandler::handleRedoKey() {
    std::cout << "Processing Redo command for screen navigation..." << std::endl;

    bool success = executeRedo();
    logCommandExecution("RedoCommand", success);

    if (success) {
        std::cout << "Redo executed successfully - moved forward in navigation!" << std::endl;
    }
    else {
        std::cout << "No actions available to redo" << std::endl;
    }

    return success;
}

bool SettingsCommandHandler::handleHistoryKey() {
    std::cout << "Displaying command history for debugging..." << std::endl;
    printCommandHistory();
    logCommandExecution("HistoryCommand", true);
    return true;
}

void SettingsCommandHandler::executeEscapeCommand() {
    std::cout << "Creating and executing EscapeKeyCommand..." << std::endl;

    auto escapeCommand = createEscapeCommand();
    executeCommandSafely(std::move(escapeCommand));

    std::cout << "Escape command dispatched to CommandInvoker" << std::endl;
}

bool SettingsCommandHandler::executeUndo() {
    return AppContext::instance().commandInvoker().undo();
}

bool SettingsCommandHandler::executeRedo() {
    return AppContext::instance().commandInvoker().redo();
}

void SettingsCommandHandler::printCommandHistory() {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "    SETTINGS SCREEN - COMMAND HISTORY" << std::endl;
    std::cout << std::string(50, '=') << std::endl;

    AppContext::instance().commandInvoker().printHistory();

    std::cout << std::string(50, '=') << std::endl;
    std::cout << "    END OF COMMAND HISTORY" << std::endl;
    std::cout << std::string(50, '=') << "\n" << std::endl;
}

void SettingsCommandHandler::saveSettingsBeforeExit() {
    std::cout << "Auto-saving before exit..." << std::endl;

    bool volumeSaved = false;
    bool languageSaved = false;

    // Save volume settings
    if (auto panel = m_volumePanel.lock()) {
        if (panel->hasChanged()) {
            try {
                panel->saveSettings();
                volumeSaved = true;
                std::cout << "Volume saved" << std::endl;
            }
            catch (const std::exception& e) {
                std::cout << "Volume save failed: " << e.what() << std::endl;
            }
        }
    }

    // Save language settings
    if (auto panel = m_languagePanel.lock()) {
        if (panel->hasChanged()) {
            try {
                panel->saveLanguagePreference();
                languageSaved = true;
                std::cout << "Language saved" << std::endl;
            }
            catch (const std::exception& e) {
                std::cout << "Language save failed: " << e.what() << std::endl;
            }
        }
    }

    if (!volumeSaved && !languageSaved) {
        std::cout << "No changes to save" << std::endl;
    }
}

bool SettingsCommandHandler::shouldSaveVolumeSettings() {
    if (auto panel = m_volumePanel.lock()) {
        return panel->hasChanged();
    }
    return false;
}

bool SettingsCommandHandler::shouldSaveLanguageSettings() {
    if (auto panel = m_languagePanel.lock()) {
        return panel->hasChanged();
    }
    return false;
}

void SettingsCommandHandler::performAutoSave() {
    if (m_autoSaveEnabled) {
        std::cout << "Performing scheduled auto-save..." << std::endl;
        saveSettingsBeforeExit();
    }
}

void SettingsCommandHandler::setVolumePanel(std::shared_ptr<VolumeControlPanel> panel) {
    m_volumePanel = panel;
    std::cout << "Volume panel registered with command handler for auto-save" << std::endl;
}

void SettingsCommandHandler::setLanguagePanel(std::shared_ptr<LanguageControlPanel> panel) {
    m_languagePanel = panel;
    std::cout << "Language panel registered with command handler for auto-save" << std::endl;
}

std::unique_ptr<EscapeKeyCommand> SettingsCommandHandler::createEscapeCommand() {
    return std::make_unique<EscapeKeyCommand>(
        ScreenType::SETTINGS,
        ScreenType::MENU
    );
}

void SettingsCommandHandler::executeCommandSafely(std::unique_ptr<ICommand> command) {
    try {
        if (command) {
            AppContext::instance().commandInvoker().execute(std::move(command));
        }
        else {
            std::cout << "Error: Null command passed to executeCommandSafely" << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cout << "Exception during command execution: " << e.what() << std::endl;
    }
}

bool SettingsCommandHandler::isValidKeyboardEvent(const sf::Event& event) const {
    return event.type == sf::Event::KeyPressed;
}

bool SettingsCommandHandler::isModifierKeyPressed(const sf::Event& event) const {
    return event.key.control || event.key.alt || event.key.shift;
}

void SettingsCommandHandler::logCommandExecution(const std::string& commandName, bool success) {
    std::string status = success ? "SUCCESS" : "FAILED";
    std::string timestamp = ""; // In real implementation, add timestamp

    std::cout << "[COMMAND] " << commandName << " - " << status << std::endl;

    // In a real application, this could write to a log file
    // or send to a monitoring system
}

void SettingsCommandHandler::logInputEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        std::cout << "[INPUT] Key pressed: " << event.key.code;
        if (isModifierKeyPressed(event)) {
            std::cout << " (with modifiers)";
        }
        std::cout << std::endl;
    }
}

void SettingsCommandHandler::logAutoSaveAction(const std::string& action) {
    std::cout << "[AUTO-SAVE] " << action << std::endl;
}
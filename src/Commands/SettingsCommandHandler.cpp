#include "../../include/Commands/SettingsCommandHandler.h"
#include "../UI/VolumeControlPanel.h"
#include <iostream>

SettingsCommandHandler::SettingsCommandHandler() {}

bool SettingsCommandHandler::handleKeyboardInput(const sf::Event& event) {
    // Validate input
    if (event.type != sf::Event::KeyPressed) {
        return false;
    }

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
        return true;

    }
    catch (const std::exception& e) {
        std::cout << "Error executing escape: " << e.what() << std::endl;
        return false;
    }
}
bool SettingsCommandHandler::handleUndoKey() {
    bool success = executeUndo();
    logCommandExecution("UndoCommand", success);

    return success;
}

bool SettingsCommandHandler::handleRedoKey() {
    bool success = executeRedo();
    logCommandExecution("RedoCommand", success);

    return success;
}

bool SettingsCommandHandler::handleHistoryKey() {
    printCommandHistory();
    logCommandExecution("HistoryCommand", true);
    return true;
}

void SettingsCommandHandler::executeEscapeCommand() {
    auto escapeCommand = createEscapeCommand();
    executeCommandSafely(std::move(escapeCommand));
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
    bool volumeSaved = false;
    bool languageSaved = false;

    // Save volume settings
    if (auto panel = m_volumePanel.lock()) {
        if (panel->hasChanged()) {
            try {
                panel->saveSettings();
                volumeSaved = true;
            }
            catch (const std::exception& e) {
                std::cout << "Volume save failed: " << e.what() << std::endl;
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

void SettingsCommandHandler::performAutoSave() {
    if (m_autoSaveEnabled) {
        saveSettingsBeforeExit();
    }
}

void SettingsCommandHandler::setVolumePanel(std::shared_ptr<VolumeControlPanel> panel) {
    m_volumePanel = panel;
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
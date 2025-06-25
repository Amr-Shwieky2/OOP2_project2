#include "../Settings/SettingsCommandExecutor.h"
#include "../Application/AppContext.h"
#include <iostream>

SettingsCommandExecutor::SettingsCommandExecutor() {
    std::cout << "SettingsCommandExecutor: Initialized for command execution only" << std::endl;
}

void SettingsCommandExecutor::executeEscapeCommand() {
    try {
        std::cout << "SettingsCommandExecutor: Creating and executing Escape command" << std::endl;

        auto escapeCommand = createEscapeCommand();
        executeCommandSafely(std::move(escapeCommand));

    }
    catch (const std::exception& e) {
        handleCommandError("EscapeCommand", e);
    }
}

bool SettingsCommandExecutor::executeUndo() {
    try {
        std::cout << "SettingsCommandExecutor: Executing Undo command" << std::endl;

        bool result = AppContext::instance().commandInvoker().undo();

        if (result) {
            std::cout << "SettingsCommandExecutor: Undo executed successfully" << std::endl;
        }
        else {
            std::cout << "SettingsCommandExecutor: No command available to undo" << std::endl;
        }

        return result;

    }
    catch (const std::exception& e) {
        handleCommandError("UndoCommand", e);
        return false;
    }
}

bool SettingsCommandExecutor::executeRedo() {
    try {
        std::cout << "SettingsCommandExecutor: Executing Redo command" << std::endl;

        bool result = AppContext::instance().commandInvoker().redo();

        if (result) {
            std::cout << "SettingsCommandExecutor: Redo executed successfully" << std::endl;
        }
        else {
            std::cout << "SettingsCommandExecutor: No command available to redo" << std::endl;
        }

        return result;

    }
    catch (const std::exception& e) {
        handleCommandError("RedoCommand", e);
        return false;
    }
}

void SettingsCommandExecutor::executeHistoryCommand() {
    try {
        std::cout << "SettingsCommandExecutor: Displaying command history" << std::endl;
        std::cout << "\n" << std::string(50, '=') << std::endl;
        std::cout << "    SETTINGS SCREEN - COMMAND HISTORY" << std::endl;
        std::cout << std::string(50, '=') << std::endl;

        AppContext::instance().commandInvoker().printHistory();

        std::cout << std::string(50, '=') << std::endl;
        std::cout << "    END OF COMMAND HISTORY" << std::endl;
        std::cout << std::string(50, '=') << "\n" << std::endl;

    }
    catch (const std::exception& e) {
        handleCommandError("HistoryCommand", e);
    }
}

void SettingsCommandExecutor::clearCommandHistory() {
    try {
        std::cout << "SettingsCommandExecutor: Clearing command history" << std::endl;
        AppContext::instance().commandInvoker().clearHistory();

    }
    catch (const std::exception& e) {
        handleCommandError("ClearHistoryCommand", e);
    }
}

bool SettingsCommandExecutor::canUndo() const {
    try {
        return AppContext::instance().commandInvoker().canUndo();
    }
    catch (const std::exception& e) {
        std::cout << "SettingsCommandExecutor: Error checking undo status: " << e.what() << std::endl;
        return false;
    }
}

bool SettingsCommandExecutor::canRedo() const {
    try {
        return AppContext::instance().commandInvoker().canRedo();
    }
    catch (const std::exception& e) {
        std::cout << "SettingsCommandExecutor: Error checking redo status: " << e.what() << std::endl;
        return false;
    }
}

void SettingsCommandExecutor::executeCommandSafely(std::unique_ptr<ICommand> command) {
    if (!command) {
        std::cout << "SettingsCommandExecutor: Warning - Null command provided" << std::endl;
        return;
    }

    try {
        std::cout << "SettingsCommandExecutor: Executing command: " << command->getName() << std::endl;
        AppContext::instance().commandInvoker().execute(std::move(command));

    }
    catch (const std::exception& e) {
        std::cout << "SettingsCommandExecutor: Exception during command execution: " << e.what() << std::endl;
    }
}

std::unique_ptr<EscapeKeyCommand> SettingsCommandExecutor::createEscapeCommand() {
    return std::make_unique<EscapeKeyCommand>(
        ScreenType::SETTINGS,
        ScreenType::MENU
    );
}

void SettingsCommandExecutor::handleCommandError(const std::string& commandName, const std::exception& e) {
    std::cout << "SettingsCommandExecutor: Error executing " << commandName
        << ": " << e.what() << std::endl;
}
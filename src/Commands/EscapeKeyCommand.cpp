#include "../../include/Commands/EscapeKeyCommand.h"
#include "../../include/Application/AppContext.h"
#include <iostream>

EscapeKeyCommand::EscapeKeyCommand(ScreenType currentScreen, ScreenType targetScreen)
    : m_currentScreen(currentScreen), m_targetScreen(targetScreen) {

    std::cout << "EscapeKeyCommand: Created - "
        << screenTypeToString(m_currentScreen)
        << " -> " << screenTypeToString(m_targetScreen) << std::endl;
}

void EscapeKeyCommand::execute() {
    std::cout << "EscapeKeyCommand: Executing..." << std::endl;

    try {
        // Get screen manager
        AppContext::instance().screenManager().changeScreen(m_targetScreen);

        // Mark as executed
        m_hasExecuted = true;

        std::cout << "EscapeKeyCommand: Success!" << std::endl;

    }
    catch (const std::exception& e) {
        std::cout << "EscapeKeyCommand: Failed - " << e.what() << std::endl;
        m_hasExecuted = false;
        throw; // Re-throw for upper level handling
    }
}

void EscapeKeyCommand::undo() {
    if (!m_hasExecuted) {
        std::cout << "EscapeKeyCommand: Cannot undo - command was not executed" << std::endl;
        return;
    }

    std::cout << "EscapeKeyCommand: Undoing - Returning to "
        << screenTypeToString(m_currentScreen) << std::endl;

    try {
        // العودة للشاشة السابقة
        AppContext::instance().screenManager().changeScreen(m_currentScreen);
        std::cout << "EscapeKeyCommand: Undo successful!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "EscapeKeyCommand: Failed to undo - " << e.what() << std::endl;
    }
}

bool EscapeKeyCommand::canUndo() const {
    // يمكن التراجع إذا تم التنفيذ والشاشات مختلفة
    return m_hasExecuted && (m_currentScreen != m_targetScreen);
}

std::string EscapeKeyCommand::getName() const {
    return "EscapeKey(" + screenTypeToString(m_currentScreen) +
        " -> " + screenTypeToString(m_targetScreen) + ")";
}

std::string EscapeKeyCommand::screenTypeToString(ScreenType screen) const {
    switch (screen) {
    case ScreenType::LOADING:   return "LOADING";
    case ScreenType::MENU:      return "MENU";
    case ScreenType::PLAY:      return "PLAY";
    case ScreenType::HELP:      return "HELP";
    case ScreenType::SETTINGS:  return "SETTINGS";
    case ScreenType::GAMEOVER:  return "GAMEOVER";
    case ScreenType::WINNING:   return "WINNING";
    case ScreenType::ABOUT_US:  return "ABOUT_US";
    case ScreenType::PAUSE:     return "PAUSE";
    default:                    return "UNKNOWN";
    }
}

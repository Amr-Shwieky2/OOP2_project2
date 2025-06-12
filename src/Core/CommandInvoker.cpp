#include "../../include/Core/CommandInvoker.h"

CommandInvoker::CommandInvoker() : m_currentIndex(-1) {
    std::cout << "CommandInvoker: Initialized" << std::endl;
}

void CommandInvoker::execute(std::unique_ptr<ICommand> command) {
    if (!command) {
        std::cout << "CommandInvoker: Null command received!" << std::endl;
        return;
    }

    std::cout << "CommandInvoker: Executing command '" << command->getName() << "'" << std::endl;

    // Execute the command
    command->execute();

    // Only save undoable commands to history
    if (command->canUndo()) {
        // Remove any commands after current position (clears redo stack)
        if (m_currentIndex + 1 < m_commandHistory.size()) {
            m_commandHistory.erase(m_commandHistory.begin() + m_currentIndex + 1,
                m_commandHistory.end());
        }

        // Add new command to history
        m_commandHistory.push_back(std::move(command));
        m_currentIndex = m_commandHistory.size() - 1;

        // Clean up if history is too large
        cleanupHistory();

        std::cout << "CommandInvoker: Command saved to history (index: " << m_currentIndex << ")" << std::endl;
    }
    else {
        std::cout << "CommandInvoker: Command executed but not saved (not undoable)" << std::endl;
    }
}

bool CommandInvoker::undo() {
    if (!canUndo()) {
        std::cout << "CommandInvoker: Cannot undo - no commands in history" << std::endl;
        return false;
    }

    // Get current command and undo it
    auto& command = m_commandHistory[m_currentIndex];
    std::cout << "CommandInvoker: Undoing command '" << command->getName() << "'" << std::endl;

    command->undo();
    m_currentIndex--;  // Move backward in history

    std::cout << "CommandInvoker: Undo successful (new index: " << m_currentIndex << ")" << std::endl;
    return true;
}

bool CommandInvoker::redo() {
    if (!canRedo()) {
        std::cout << "CommandInvoker: Cannot redo - no commands to redo" << std::endl;
        return false;
    }

    // Move forward in history and re-execute command
    m_currentIndex++;
    auto& command = m_commandHistory[m_currentIndex];
    std::cout << "CommandInvoker: Redoing command '" << command->getName() << "'" << std::endl;

    command->execute();

    std::cout << "CommandInvoker: Redo successful (new index: " << m_currentIndex << ")" << std::endl;
    return true;
}

bool CommandInvoker::canUndo() const {
    // Can undo if we have commands and current index is valid
    return m_currentIndex >= 0 && !m_commandHistory.empty();
}

bool CommandInvoker::canRedo() const {
    // Can redo if there are commands ahead of current position
    return m_currentIndex + 1 < m_commandHistory.size();
}

void CommandInvoker::clearHistory() {
    std::cout << "CommandInvoker: Clearing command history" << std::endl;
    m_commandHistory.clear();
    m_currentIndex = -1;
}

void CommandInvoker::printHistory() const {
    std::cout << "CommandInvoker: Command History:" << std::endl;
    std::cout << "   Total commands: " << m_commandHistory.size() << std::endl;
    std::cout << "   Current index: " << m_currentIndex << std::endl;

    for (size_t i = 0; i < m_commandHistory.size(); i++) {
        std::string marker = (i == m_currentIndex) ? " <- Current" : "";
        std::cout << "   [" << i << "] " << m_commandHistory[i]->getName() << marker << std::endl;
    }
}

void CommandInvoker::cleanupHistory() {
    if (m_commandHistory.size() > MAX_HISTORY_SIZE) {
        std::cout << "CommandInvoker: Cleaning up history (limit: " << MAX_HISTORY_SIZE << ")" << std::endl;

        // Keep only the last MAX_HISTORY_SIZE commands
        size_t removeCount = m_commandHistory.size() - MAX_HISTORY_SIZE;
        m_commandHistory.erase(m_commandHistory.begin(),
            m_commandHistory.begin() + removeCount);

        // Adjust current index
        m_currentIndex -= removeCount;

        std::cout << "CommandInvoker: History cleaned up (new size: " << m_commandHistory.size() << ")" << std::endl;
    }
}
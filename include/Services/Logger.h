#pragma once

#include <string>
#include <fstream>
#include <mutex>
#include <memory>
#include <iomanip>   // for std::put_time
#include <sstream>   // for std::ostringstream
#include <ctime>     // for std::time, std::localtime

/**
 * Logger is a simple thread‐safe singleton that writes timestamped messages
 * to both the console and (optionally) to a file.
 *
 * Usage:
 *   Logger::instance().logInfo("Starting game...");
 *   Logger::instance().logError("Could not load texture");
 */
class Logger {
public:
    enum class Level {
        Debug,
        Info,
        Warning,
        Error
    };

    // Get the singleton Logger instance
    static Logger& instance();

    // Delete copy/move so no accidental duplicates:
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(Logger&&) = delete;

    // Open a log file (if you want to write to disk). Call early in main.
    // If filename is empty, logs go only to console.
    void openLogFile(const std::string& filename);

    // Close the current log file (if open)
    void closeLogFile();

    // Log a message at different severity levels:
    void logDebug(const std::string& message);
    void logInfo(const std::string& message);
    void logWarning(const std::string& message);
    void logError(const std::string& message);

private:
    Logger();   // Private ctor for singleton
    ~Logger();

    // Internal helper to format and dispatch to output streams
    void log(Level level, const std::string& message);

    std::mutex       m_mutex;       // Guards writes to file & console
    std::ofstream    m_fileStream;  // If open, write logs here
    bool             m_fileOpen;    // True if m_fileStream is currently open
};

#include "Logger.h"
#include <iostream>
#include <ctime>
#include <iomanip>

//-------------------------------------------
// 1) Singleton accessor
//-------------------------------------------
Logger& Logger::instance() {
    static Logger singletonInstance;
    return singletonInstance;
}

//-------------------------------------------
// 2) Constructor & Destructor
//-------------------------------------------
Logger::Logger()
    : m_fileOpen(false)
{
    // By default, no file is open: logs go to stdout only.
}

Logger::~Logger() {
    closeLogFile();
}

//-------------------------------------------
// 3) openLogFile / closeLogFile
//-------------------------------------------
void Logger::openLogFile(const std::string& filename) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_fileOpen) {
        m_fileStream.close();
        m_fileOpen = false;
    }

    m_fileStream.open(filename, std::ios::out | std::ios::app);
    if (m_fileStream.fail()) {
        m_fileOpen = false;
        std::cerr << "[Logger] Failed to open log file: " << filename << "\n";
    }
    else {
        m_fileOpen = true;
        logInfo("Log file opened: " + filename);
    }
}

void Logger::closeLogFile() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_fileOpen) {
        logInfo("Closing log file.");
        m_fileStream.close();
        m_fileOpen = false;
    }
}

//-------------------------------------------
// 4) Public logging methods
//-------------------------------------------
void Logger::logDebug(const std::string& message) {
    log(Level::Debug, message);
}

void Logger::logInfo(const std::string& message) {
    log(Level::Info, message);
}

void Logger::logWarning(const std::string& message) {
    log(Level::Warning, message);
}

void Logger::logError(const std::string& message) {
    log(Level::Error, message);
}

//-------------------------------------------
// 5) Internal helper: formatting & writing
//-------------------------------------------
void Logger::log(Level level, const std::string& message) {
    std::lock_guard<std::mutex> lock(m_mutex);

    // 5.1) Build a timestamp:
    auto t = std::time(nullptr);
    std::tm tm{};
#if defined(_MSC_VER) || defined(__MINGW32__)
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif

    std::ostringstream prefix;
    prefix << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

    // 5.2) Determine level string
    const char* levelStr = "";
    switch (level) {
    case Level::Debug:   levelStr = "[DEBUG] ";   break;
    case Level::Info:    levelStr = "[INFO ] ";   break;
    case Level::Warning: levelStr = "[WARN ] ";   break;
    case Level::Error:   levelStr = "[ERROR] ";   break;
    }

    // 5.3) Construct full line
    std::string line = prefix.str() + " " + levelStr + message;

    // 5.4) Write to console (stderr for warnings/errors)
    if (level == Level::Error || level == Level::Warning) {
        std::cerr << line << "\n";
    }
    else {
        std::cout << line << "\n";
    }

    // 5.5) If a file is open, append to it
    if (m_fileOpen) {
        m_fileStream << line << "\n";
        // flush now so you don’t lose logs if crash occurs
        m_fileStream.flush();
    }
}

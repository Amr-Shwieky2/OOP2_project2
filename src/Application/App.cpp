#include "App.h"
#include "AppContext.h"
#include "ScreenTypes.h"
#include "Logger.h"
#include "LanguageManager.h"
#include "MultiFontManager.h"

// Include screen files
#include "LoadingScreen.h"
#include "MenuScreen.h"
#include "GameplayScreen.h"
#include "SettingsScreen.h"
#include "AboutScreen.h"


#include<fstream>
#include "../../include/Screens/HelpScreen.h"

App::App() {
    // Create window
    m_window = std::make_unique<sf::RenderWindow>(
        sf::VideoMode(1400, 800), "Desert Ball"
    );
    m_window->setFramerateLimit(60);
}

void App::run() {
    Logger::log("Starting Desert Ball game...");
    initialize();
    mainLoop();
    Logger::log("Game ended.");
}

void App::initialize() {
    // Initialize language system first
    initializeLanguageSystem();

    auto& screenManager = AppContext::instance().screenManager();

    // Register all screens
    screenManager.registerScreen(ScreenType::LOADING, []() {
        return std::make_unique<LoadingScreen>();
        });

    screenManager.registerScreen(ScreenType::MENU, []() {
        return std::make_unique<MenuScreen>();
        });

    screenManager.registerScreen(ScreenType::PLAY, []() {
        return std::make_unique<GameplayScreen>();
        });

    screenManager.registerScreen(ScreenType::SETTINGS, []() {
        return std::make_unique<SettingsScreen>();
        });

    screenManager.registerScreen(ScreenType::HELP, []() {
        return std::make_unique<HelpScreen>();
        });
    screenManager.registerScreen(ScreenType::ABOUT_US, []() {
        return std::make_unique<AboutScreen>();
        });
    // Start with loading screen
    screenManager.changeScreen(ScreenType::LOADING);
    Logger::log("All screens initialized");
}

void App::initializeLanguageSystem() {
    try {
        Logger::log("Initializing language system...");

        // Load fonts for all languages
        MultiFontManager::instance().loadLanguageFonts();

        // Load language files
        auto& langManager = LanguageManager::instance();

        langManager.loadLanguageFile(Language::ENGLISH, "english.txt");
        langManager.loadLanguageFile(Language::ARABIC, "arabic.txt");
        langManager.loadLanguageFile(Language::HEBREW, "hebrew.txt");

        // Load saved language preference
        loadLanguagePreference();

        Logger::log("Language system initialized successfully");
    }
    catch (const std::exception& e) {
        Logger::log("Error initializing language system: " + std::string(e.what()), LogLevel::Error);
        // Fallback to English
        LanguageManager::instance().setLanguage(Language::ENGLISH);
    }
}

void App::loadLanguagePreference() {
    try {
        std::ifstream file("language.txt");
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                if (line.length() == 0) continue;

                size_t pos = line.find('=');
                if (pos != std::string::npos && pos > 0 && pos < line.length() - 1) {
                    std::string key = line.substr(0, pos);
                    std::string valueStr = line.substr(pos + 1);

                    if (key == "language" && !valueStr.empty()) {
                        int value = std::stoi(valueStr);
                        if (value >= 0 && value <= 2) {
                            Language savedLang = static_cast<Language>(value);
                            LanguageManager::instance().setLanguage(savedLang);
                            Logger::log("Loaded saved language: " + std::to_string(value));
                            return;
                        }
                    }
                }
            }
            file.close();
        }

        // Default to English if no saved preference
        LanguageManager::instance().setLanguage(Language::ENGLISH);
        Logger::log("Using default language: English");
    }
    catch (const std::exception& e) {
        Logger::log("Error loading language preference: " + std::string(e.what()), LogLevel::Warning);
        LanguageManager::instance().setLanguage(Language::ENGLISH);
    }
}

void App::mainLoop() {
    sf::Clock clock;
    auto& screenManager = AppContext::instance().screenManager();

    while (m_window->isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        // Handle events
        screenManager.handleEvents(*m_window);

        // Update game
        screenManager.update(deltaTime);

        // Draw everything
        m_window->clear(sf::Color::Black);
        screenManager.render(*m_window);
        m_window->display();
    }
}
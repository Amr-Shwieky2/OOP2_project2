#pragma once
#include <string>
#include <exception>

/**
 * @brief Game system initialization only - follows SRP
 */
class GameInitializer {
public:
    GameInitializer() = default;
    ~GameInitializer() = default;

    // Main initialization entry point
    void initializeAllSystems();

private:
    // Individual system initializers
    void initializeAudioSystem();
    void initializeResourceSystem();
    void registerAllScreens();

    // Helper methods
    void loadDefaultAudioFiles();
    void setDefaultAudioVolumes();
    void registerScreenFactories();

    // FIXED: Correct function signature (2 parameters)
    void handleInitializationError(const std::string& system, const std::string& error);
};
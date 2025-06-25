#pragma once
#include <string>

class GameInitializer {
public:
    GameInitializer() = default;
    ~GameInitializer() = default;

    void initializeAllSystems();

private:
    void initializeAudioSystem();
    void initializeResourceSystem();
    void registerAllScreens();

    void loadDefaultAudioFiles();
    void setDefaultAudioVolumes();
    void registerScreenFactories();

    void handleInitializationError(const std::string& system, const std::string& error);
};

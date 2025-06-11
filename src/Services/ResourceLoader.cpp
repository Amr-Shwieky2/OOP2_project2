#include "ResourceLoader.h"
#include "Logger.h"     // optional: to log load successes/failures
#include <stdexcept>

//-------------------------------------------
// 1) Singleton accessor
//-------------------------------------------
ResourceLoader& ResourceLoader::instance() {
    static ResourceLoader singletonInstance;
    return singletonInstance;
}

//-------------------------------------------
// 2) Constructor
//-------------------------------------------
ResourceLoader::ResourceLoader() {
    // Nothing to do here; maps start empty.
    Logger::instance().logInfo("ResourceLoader initialized.");
}

//-------------------------------------------
// 3) loadTexture
//-------------------------------------------
void ResourceLoader::loadTexture(const std::string& key, const std::string& filename) {
    std::lock_guard<std::mutex> lock(m_mutex);

    // If already loaded, skip reloading (or reload if you prefer).
    if (m_textures.find(key) != m_textures.end()) {
        Logger::instance().logWarning("ResourceLoader::loadTexture(): key \"" + key + "\" already exists; skipping load.");
        return;
    }

    sf::Texture texture;
    if (!texture.loadFromFile(filename)) {
        Logger::instance().logError("ResourceLoader::loadTexture(): failed to load \"" + filename + "\"");
        throw std::runtime_error("Failed to load texture: " + filename);
    }

    // Insert into map (move the newly loaded texture in)
    m_textures.emplace(key, std::move(texture));
    Logger::instance().logInfo("ResourceLoader: loaded texture \"" + key + "\" from " + filename);
}

//-------------------------------------------
// 4) getTexture
//-------------------------------------------
sf::Texture& ResourceLoader::getTexture(const std::string& key) {
    std::lock_guard<std::mutex> lock(m_mutex);

    auto it = m_textures.find(key);
    if (it == m_textures.end()) {
        Logger::instance().logError("ResourceLoader::getTexture(): key \"" + key + "\" not found");
        throw std::out_of_range("Texture key not found: " + key);
    }
    return it->second;
}

//-------------------------------------------
// 5) loadFont
//-------------------------------------------
void ResourceLoader::loadFont(const std::string& key, const std::string& filename) {
    std::lock_guard<std::mutex> lock(m_mutex);

    // If already loaded, skip reloading.
    if (m_fonts.find(key) != m_fonts.end()) {
        Logger::instance().logWarning("ResourceLoader::loadFont(): key \"" + key + "\" already exists; skipping load.");
        return;
    }

    sf::Font font;
    if (!font.loadFromFile(filename)) {
        Logger::instance().logError("ResourceLoader::loadFont(): failed to load \"" + filename + "\"");
        throw std::runtime_error("Failed to load font: " + filename);
    }

    m_fonts.emplace(key, std::move(font));
    Logger::instance().logInfo("ResourceLoader: loaded font \"" + key + "\" from " + filename);
}

//-------------------------------------------
// 6) getFont
//-------------------------------------------
sf::Font& ResourceLoader::getFont(const std::string& key) {
    std::lock_guard<std::mutex> lock(m_mutex);

    auto it = m_fonts.find(key);
    if (it == m_fonts.end()) {
        Logger::instance().logError("ResourceLoader::getFont(): key \"" + key + "\" not found");
        throw std::out_of_range("Font key not found: " + key);
    }
    return it->second;
}

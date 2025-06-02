#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>
#include <mutex>

/**
 * ResourceLoader is a singleton that manages:
 *   • Loading and caching textures (sf::Texture)
 *   • Loading and caching fonts (sf::Font)
 *
 * Usage:
 *   ResourceLoader::instance().loadTexture("player", "assets/img/player.png");
 *   sf::Texture& tex = ResourceLoader::instance().getTexture("player");
 *
 *   ResourceLoader::instance().loadFont("arial", "assets/fonts/arial.ttf");
 *   sf::Font& font = ResourceLoader::instance().getFont("arial");
 */
class ResourceLoader {
public:
    // Retrieve the sole instance
    static ResourceLoader& instance();

    // Delete copy/move so no accidental duplicates:
    ResourceLoader(const ResourceLoader&) = delete;
    ResourceLoader& operator=(const ResourceLoader&) = delete;
    ResourceLoader(ResourceLoader&&) = delete;
    ResourceLoader& operator=(ResourceLoader&&) = delete;
    ~ResourceLoader() = default;


    // Load a texture from file and associate it with the given key.
    // Throws std::runtime_error if loading fails.
    void loadTexture(const std::string& key, const std::string& filename);

    // Retrieve a reference to an already loaded texture.
    // Throws std::out_of_range if key is not found.
    sf::Texture& getTexture(const std::string& key);

    // Load a font from file and associate it with the given key.
    // Throws std::runtime_error if loading fails.
    void loadFont(const std::string& key, const std::string& filename);

    // Retrieve a reference to an already loaded font.
    // Throws std::out_of_range if key is not found.
    sf::Font& getFont(const std::string& key);

private:
    ResourceLoader();   // private ctor

    // Internal maps and mutex to guard them
    std::mutex                                      m_mutex;
    std::unordered_map<std::string, sf::Texture>    m_textures;
    std::unordered_map<std::string, sf::Font>       m_fonts;
};

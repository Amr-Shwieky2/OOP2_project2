#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <memory>

// Manages loading and caching of SFML resources like textures and fonts
class ResourceLoader {
public:
    ResourceLoader() = default;
    ~ResourceLoader() = default;

    // Load or retrieve a cached texture
    sf::Texture& getTexture(const std::string& filename);

    // Load or retrieve a cached font
    sf::Font& getFont(const std::string& filename);

private:
    std::unordered_map<std::string, std::unique_ptr<sf::Texture>> m_textures;
    std::unordered_map<std::string, std::unique_ptr<sf::Font>> m_fonts;
};

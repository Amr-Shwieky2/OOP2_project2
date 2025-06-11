#include "ResourceLoader.h"
#include <stdexcept>

sf::Texture& ResourceLoader::getTexture(const std::string& filename) {
    auto it = m_textures.find(filename);
    if (it != m_textures.end()) {
        return *it->second;
    }

    auto texture = std::make_unique<sf::Texture>();
    if (!texture->loadFromFile(filename)) {
        throw std::runtime_error("Failed to load texture: " + filename);
    }

    sf::Texture& ref = *texture;
    m_textures[filename] = std::move(texture);
    return ref;
}

sf::Font& ResourceLoader::getFont(const std::string& filename) {
    auto it = m_fonts.find(filename);
    if (it != m_fonts.end()) {
        return *it->second;
    }

    auto font = std::make_unique<sf::Font>();
    if (!font->loadFromFile(filename)) {
        throw std::runtime_error("Failed to load font: " + filename);
    }

    sf::Font& ref = *font;
    m_fonts[filename] = std::move(font);
    return ref;
}

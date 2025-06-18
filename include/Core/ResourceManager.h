#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include <stdexcept>

// Template class for managing any type of resource
template<typename ResourceType>
class ResourceManager {
public:
    ResourceManager() = default;
    ~ResourceManager() = default;

    // Load and cache a resource
    ResourceType& getResource(const std::string& filename) {
        auto it = m_resources.find(filename);
        if (it != m_resources.end()) {
            return *it->second;
        }

        auto resource = std::make_unique<ResourceType>();
        if (!loadResource(*resource, filename)) {
            throw std::runtime_error("Failed to load resource: " + filename);
        }

        ResourceType& ref = *resource;
        m_resources[filename] = std::move(resource);
        return ref;
    }

    // Check if resource is loaded
    bool hasResource(const std::string& filename) const {
        return m_resources.find(filename) != m_resources.end();
    }

    // Get resource count
    size_t getResourceCount() const {
        return m_resources.size();
    }

    // Clear all resources
    void clear() {
        m_resources.clear();
    }

    // Template specialization will be used for different resource types
private:
    std::unordered_map<std::string, std::unique_ptr<ResourceType>> m_resources;

    // This function needs to be specialized for each resource type
    bool loadResource(ResourceType& resource, const std::string& filename);
};

// Template specializations for SFML types
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

// Specialization for sf::Texture
template<>
inline bool ResourceManager<sf::Texture>::loadResource(sf::Texture& texture, const std::string& filename) {
    return texture.loadFromFile(filename);
}

// Specialization for sf::Font
template<>
inline bool ResourceManager<sf::Font>::loadResource(sf::Font& font, const std::string& filename) {
    return font.loadFromFile(filename);
}

// Specialization for sf::SoundBuffer
template<>
inline bool ResourceManager<sf::SoundBuffer>::loadResource(sf::SoundBuffer& soundBuffer, const std::string& filename) {
    return soundBuffer.loadFromFile(filename);
}

// Type aliases for convenience
using TextureManager = ResourceManager<sf::Texture>;
using FontManager = ResourceManager<sf::Font>;
using SoundManager = ResourceManager<sf::SoundBuffer>;
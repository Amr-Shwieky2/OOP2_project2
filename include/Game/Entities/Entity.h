#pragma once

#include <vector>
#include <memory>
#include <cstdint>
#include <SFML/Graphics.hpp>

/**
 * Forward‐declare Component so Entity can hold them.
 */
namespace Game::Components {
    class Component;
}

namespace Game::Entities {

    /**
     * A unique ID for each Entity instance. Helps tracking, serialization, etc.
     */
    using EntityID = std::uint32_t;

    /**
     * Base class for any in‐game “Entity.” Examples:
     *    • PlayerBall
     *    • Enemy
     *    • Box
     *    • Pyramid
     *    • MysteryBox
     *
     * Each Entity can hold zero or more Components (physics, health, AI, etc.).
     * Entities must implement update() and render() to drive per‐frame logic.
     */
    class Entity {
    public:
        Entity();
        virtual ~Entity();

        /// Returns this entity’s unique ID
        EntityID getID() const;

        /// Called once per frame: forward to all Components’ update()
        virtual void update(float deltaTime);

        /// Called once per frame to draw the entity (and possibly its components)
        virtual void render(sf::RenderTarget& target);

        /// Mark entity for removal (e.g. when its health <= 0). Actual removal is done by whoever owns the entity list.
        void destroy();

        /// Returns true if destroy() was called
        bool isDestroyed() const;

        // --- Component management ---
        /// Attach a new component (unique_ptr takes ownership)
        void addComponent(std::unique_ptr<Components::Component> comp);

        /// Retrieve the first component of type T, or nullptr if none. T must derive from Component.
        template<typename T>
        T* getComponent();

        /// Remove (and delete) the first component of type T, if it exists.
        template<typename T>
        void removeComponent();

    protected:
        /// Called by Entity::update() to give subclasses a chance to run custom logic **after** components update.
        virtual void onUpdate(float deltaTime);

        /// Called by Entity::render() to let subclasses draw their own sprites/colliders etc.
        virtual void onRender(sf::RenderTarget& target);

    private:
        EntityID m_id;   // assigned automatically
        bool     m_destroyed;

        std::vector<std::unique_ptr<Components::Component>> m_components;

        // Helper to assign unique IDs
        static EntityID generateID();
    };

} // namespace Game::Entities

#include "Entity.inl"   // include template definitions

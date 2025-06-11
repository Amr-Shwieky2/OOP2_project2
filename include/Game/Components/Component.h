#pragma once

#include <SFML/Graphics.hpp>

namespace Game::Entities {
    class Entity;  // forward declare
}

namespace Game::Components {

    /**
     * Base class for all Components (attached to Entities).
     * Subclasses can override update() and render() as needed.
     *
     * Typical components:
     *   • PhysicsComponent  (manages Box2D body)
     *   • HealthComponent   (tracks hit points, death, etc.)
     *   • AIComponent       (enemy logic, pathfinding)
     *   • CollisionComponent (bounding‐box or shape info)
     *   • GraphicsComponent (sprite, animations)
     */
    class Component {
    public:
        Component();
        virtual ~Component();

        /// Called once per frame, after entity update starts
        virtual void update(float deltaTime);

        /// Called during entity’s render() pass
        virtual void render(sf::RenderTarget& target);

        /// Assign which Entity “owns” this component
        void setOwner(Entities::Entity* owner);

        /// Retrieve pointer to owner
        Entities::Entity* getOwner() const;

    protected:
        Entities::Entity* m_owner;  // not owned; points to actual Entity
    };

} // namespace Game::Components

#include "Entity.h"
#include "Component.h"   // so we know about Components

namespace Game::Entities {

    // static member initialization
    EntityID Entity::generateID() {
        static EntityID s_nextID = 1;
        return s_nextID++;
    }

    Entity::Entity()
        : m_id(generateID()),
        m_destroyed(false)
    {
    }

    Entity::~Entity() = default;

    EntityID Entity::getID() const {
        return m_id;
    }

    void Entity::destroy() {
        m_destroyed = true;
    }

    bool Entity::isDestroyed() const {
        return m_destroyed;
    }

    void Entity::addComponent(std::unique_ptr<Components::Component> comp) {
        // Give the component a back‐pointer to this entity:
        comp->setOwner(this);
        m_components.push_back(std::move(comp));
    }

    void Entity::update(float deltaTime) {
        // 1) First let every attached component update
        for (auto& compPtr : m_components) {
            compPtr->update(deltaTime);
        }
        // 2) Then let the subclass run any custom logic
        onUpdate(deltaTime);
    }

    void Entity::render(sf::RenderTarget& target) {
        // 1) First let subclass draw any sprites, shapes, etc.
        onRender(target);
        // 2) Optionally, components might want to render debug info, collisions, etc.
        for (auto& compPtr : m_components) {
            compPtr->render(target);
        }
    }

    void Entity::onUpdate(float /*deltaTime*/) {
        // Default does nothing; subclasses override.
    }

    void Entity::onRender(sf::RenderTarget& /*target*/) {
        // Default does nothing; subclasses override.
    }

} // namespace Game::Entities

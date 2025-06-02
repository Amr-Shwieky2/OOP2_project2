#include "Component.h"

namespace Game::Components {

    Component::Component()
        : m_owner(nullptr)
    {
    }

    Component::~Component() = default;

    void Component::update(float /*deltaTime*/) {
        // Default does nothing; subclasses override.
    }

    void Component::render(sf::RenderTarget& /*target*/) {
        // Default does nothing; subclasses override.
    }

    void Component::setOwner(Entities::Entity* owner) {
        m_owner = owner;
    }

    Entities::Entity* Component::getOwner() const {
        return m_owner;
    }

} // namespace Game::Components

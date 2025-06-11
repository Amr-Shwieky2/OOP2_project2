#pragma once

#include <type_traits>

namespace Game::Entities {

    template<typename T>
    T* Entity::getComponent() {
        static_assert(std::is_base_of<Components::Component, T>::value,
            "T must derive from Component");

        for (auto& compPtr : m_components) {
            if (auto casted = dynamic_cast<T*>(compPtr.get())) {
                return casted;
            }
        }
        return nullptr;
    }

    template<typename T>
    void Entity::removeComponent() {
        static_assert(std::is_base_of<Components::Component, T>::value,
            "T must derive from Component");

        for (auto it = m_components.begin(); it != m_components.end(); ++it) {
            if (dynamic_cast<T*>((*it).get())) {
                m_components.erase(it);
                return;
            }
        }
    }

} // namespace Game::Entities

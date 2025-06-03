#pragma once
#include "../Core/IRenderable.h"
#include "../Core/IUpdatable.h" 
#include "../Core/IEventHandler.h"
#include <SFML/Graphics.hpp>
#include <memory>

class Screen : public IRenderable, public IUpdatable, public IEventHandler {
public:
    virtual ~Screen() = default;

    // Lifecycle
    virtual void onEnter() {}
    virtual void onExit() {}
    virtual void onPause() {}
    virtual void onResume() {}

    // Core functionality
    void update(float deltaTime) override = 0;
    void render(sf::RenderWindow& window) const override = 0;
    void handleEvent(const sf::Event& event) override = 0;

    // State
    bool isActive() const { return m_active; }

protected:
    void setActive(bool active) { m_active = active; }

private:
    bool m_active = true;
};
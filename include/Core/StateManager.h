#pragma once
#include <stack>
#include <memory>
#include <functional>
#include <SFML/Graphics.hpp>

// Template-based State Manager
template<typename StateType>
class StateManager {
public:
    StateManager() = default;
    ~StateManager() = default;

    // Push a new state
    void pushState(std::unique_ptr<StateType> state) {
        if (!m_states.empty()) {
            m_states.top()->onPause();
        }
        m_states.push(std::move(state));
        m_states.top()->onEnter();
    }

    // Pop current state
    void popState() {
        if (!m_states.empty()) {
            m_states.top()->onExit();
            m_states.pop();

            if (!m_states.empty()) {
                m_states.top()->onResume();
            }
        }
    }

    // Replace current state
    void changeState(std::unique_ptr<StateType> state) {
        if (!m_states.empty()) {
            popState();
        }
        pushState(std::move(state));
    }

    // Get current state
    StateType* getCurrentState() {
        return m_states.empty() ? nullptr : m_states.top().get();
    }

    // Update current state
    void update(float deltaTime) {
        if (!m_states.empty()) {
            m_states.top()->update(deltaTime);
        }
    }

    // Render current state
    void render(sf::RenderWindow& window) {
        if (!m_states.empty()) {
            m_states.top()->render(window);
        }
    }

    // Handle events for current state
    void handleEvents(sf::RenderWindow& window) {
        if (!m_states.empty()) {
            m_states.top()->handleEvents(window);
        }
    }

    // Check if stack is empty
    bool isEmpty() const {
        return m_states.empty();
    }

    // Get stack size
    size_t getStackSize() const {
        return m_states.size();
    }

private:
    std::stack<std::unique_ptr<StateType>> m_states;
};

// Base State class that works with the template
class BaseState {
public:
    virtual ~BaseState() = default;

    // Lifecycle methods
    virtual void onEnter() {}
    virtual void onExit() {}
    virtual void onPause() {}
    virtual void onResume() {}

    // Core methods
    virtual void handleEvents(sf::RenderWindow& window) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;

    // State status
    bool isActive() const { return m_active; }

protected:
    void setActive(bool active) { m_active = active; }

private:
    bool m_active = true;
};

// Typedef for our game's state manager
using GameStateManager = StateManager<BaseState>;
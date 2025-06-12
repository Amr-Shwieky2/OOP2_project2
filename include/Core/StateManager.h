#pragma once
#include <stack>
#include <memory>
#include <SFML/Graphics.hpp>

/**
 * @brief Stack-based State Manager for game screens
 *
 * Manages overlapping states (screens) with pause/resume support.
 * Last In, First Out - like browser tabs or mobile app stack.
 */
template<typename StateType>
class StateManager {
public:
    // Add new state on top (pauses current)
    void pushState(std::unique_ptr<StateType> state) {
        if (!m_states.empty()) {
            m_states.top()->onPause();
        }
        m_states.push(std::move(state));
        m_states.top()->onEnter();
    }

    // Remove current state (resumes previous)
    void popState() {
        if (!m_states.empty()) {
            m_states.top()->onExit();
            m_states.pop();
            if (!m_states.empty()) {
                m_states.top()->onResume();
            }
        }
    }

    // Replace current state completely
    void changeState(std::unique_ptr<StateType> state) {
        if (!m_states.empty()) {
            popState();
        }
        pushState(std::move(state));
    }

    // Get current active state
    StateType* getCurrentState() {
        return m_states.empty() ? nullptr : m_states.top().get();
    }

    // Delegate to current state
    void update(float deltaTime) {
        if (!m_states.empty()) {
            m_states.top()->update(deltaTime);
        }
    }

    void render(sf::RenderWindow& window) {
        if (!m_states.empty()) {
            m_states.top()->render(window);
        }
    }

    void handleEvents(sf::RenderWindow& window) {
        if (!m_states.empty()) {
            m_states.top()->handleEvents(window);
        }
    }

private:
    std::stack<std::unique_ptr<StateType>> m_states;
};

/**
 * @brief Base class for all game states
 */
class BaseState {
public:
    virtual ~BaseState() = default;

    // Lifecycle hooks
    virtual void onEnter() {}   // When becomes active
    virtual void onExit() {}    // When removed
    virtual void onPause() {}   // When covered by another state
    virtual void onResume() {}  // When becomes active again

    // Core game loop
    virtual void handleEvents(sf::RenderWindow& window) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;

private:
    bool m_active = true;
};

using GameStateManager = StateManager<BaseState>;
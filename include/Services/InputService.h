#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <unordered_set>

/**
 * InputService (singleton) handles:
 *   • Polling all SFML events each frame
 *   • Tracking currently pressed keys and mouse buttons
 *   • Providing simple query methods (isKeyDown, wasKeyPressedThisFrame, etc.)
 *
 * Usage pattern:
 *   • At the very start of each frame (after window.pollEvent), call:
 *         InputService::instance().captureEvents(window);
 *   • In game logic or screens, query:
 *         if (InputService::instance().isKeyDown(sf::Keyboard::A)) { … }
 *         if (InputService::instance().wasKeyPressed(sf::Keyboard::Space)) { … }
 *         sf::Vector2i mousePos = InputService::instance().getMousePosition();
 *
 * This service must be “stepped” once per frame to clear the one‐time “pressed this frame” sets.
 */
class InputService {
public:
    // Retrieve the sole instance
    static InputService& instance();

    // Delete copy/move to enforce singleton
    InputService(const InputService&) = delete;
    InputService& operator=(const InputService&) = delete;
    InputService(InputService&&) = delete;
    InputService& operator=(InputService&&) = delete;
    ~InputService() = default;


    /**
     * Must be called once per frame, before any calls to isKeyDown/wasKeyPressed/etc.
     *   Polls SFML events from the given window.
     *   Updates internal sets: keysDown, keysJustPressed, keysJustReleased, similarly for mouse.
     */
    void captureEvents(sf::RenderWindow& window);

    // -------- Keyboard Queries ---------

    /**
     * Returns true if the given key is currently held down (in this frame).
     */
    bool isKeyDown(sf::Keyboard::Key key) const;

    /**
     * Returns true if the given key was pressed during this frame (i.e., a key‐down event occurred).
     * This “just pressed” flag is cleared at the start of the next captureEvents() call.
     */
    bool wasKeyPressed(sf::Keyboard::Key key) const;

    /**
     * Returns true if the given key was released during this frame (i.e., a key‐up event occurred).
     * This “just released” flag is cleared at the start of the next captureEvents() call.
     */
    bool wasKeyReleased(sf::Keyboard::Key key) const;

    // -------- Mouse Queries ---------

    /**
     * Returns true if the given mouse button is currently held down.
     */
    bool isMouseButtonDown(sf::Mouse::Button button) const;

    /**
     * Returns true if the given mouse button was clicked (pressed) during this frame.
     * Clears at next captureEvents().
     */
    bool wasMouseButtonPressed(sf::Mouse::Button button) const;

    /**
     * Returns true if the given mouse button was released during this frame.
     * Clears at next captureEvents().
     */
    bool wasMouseButtonReleased(sf::Mouse::Button button) const;

    /**
     * Returns the current mouse position relative to the window (updated each frame).
     */
    sf::Vector2i getMousePosition() const;

    /**
     * Returns how much the mouse wheel moved this frame (positive = up/forward, negative = down/backwards).
     * After captureEvents(), this value resets to zero until the next wheel event.
     */
    int getMouseWheelDelta() const;

private:
    InputService();

    // Internal sets to track current and “just changed” keys/buttons:
    std::unordered_set<sf::Keyboard::Key> m_keysDown;            // keys currently held
    std::unordered_set<sf::Keyboard::Key> m_keysJustPressed;     // keys that went down this frame
    std::unordered_set<sf::Keyboard::Key> m_keysJustReleased;    // keys that went up this frame

    std::unordered_set<sf::Mouse::Button> m_mouseDown;           // buttons currently held
    std::unordered_set<sf::Mouse::Button> m_mouseJustPressed;    // buttons pressed this frame
    std::unordered_set<sf::Mouse::Button> m_mouseJustReleased;   // buttons released this frame

    sf::Vector2i m_mousePosition;   // updated each frame
    int m_mouseWheelDelta;          // scroll delta for this frame

    // Helpers to reset “just pressed/released” sets at the start of each capture.
    void clearPerFrameStates();
};

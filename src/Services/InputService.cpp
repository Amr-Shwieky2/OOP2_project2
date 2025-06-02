#include "InputService.h"
#include "Logger.h"    // optional, for debugging
#include <SFML/Window/Event.hpp>

///-------------------------------------------
/// 1) Singleton accessor
///-------------------------------------------
InputService& InputService::instance() {
    static InputService singleton;
    return singleton;
}

///-------------------------------------------
/// 2) Constructor
///-------------------------------------------
InputService::InputService()
    : m_mousePosition(0, 0),
    m_mouseWheelDelta(0)
{
    // Initially, no keys or mouse buttons are down.
}

///-------------------------------------------
/// 3) clearPerFrameStates (helper)
///-------------------------------------------
//
// Clears out any “just pressed” / “just released” flags so that they only survive one frame.
//
void InputService::clearPerFrameStates() {
    m_keysJustPressed.clear();
    m_keysJustReleased.clear();
    m_mouseJustPressed.clear();
    m_mouseJustReleased.clear();
    m_mouseWheelDelta = 0;
}

///-------------------------------------------
/// 4) captureEvents
///-------------------------------------------
//
// Should be called exactly once each frame (before you query any key/mouse methods).
//   • Polls all pending SFML events.
//   • Updates keysDown, keysJustPressed, keysJustReleased, etc.
//   • Updates mousePosition and mouseWheelDelta.
//   • Clears previous frame’s “just” sets at the start.
//
void InputService::captureEvents(sf::RenderWindow& window) {
    // 4.1) Reset “just” sets from the previous frame
    clearPerFrameStates();

    // 4.2) Poll events from the window
    sf::Event event;
    while (window.pollEvent(event)) {
        switch (event.type) {
            // Handle window close elsewhere (in App or ScreenManager). We just track input here.

            /// -- Keyboard events --
        case sf::Event::KeyPressed: {
            sf::Keyboard::Key key = event.key.code;
            // If not already marked as down, mark “just pressed”.
            if (m_keysDown.find(key) == m_keysDown.end()) {
                m_keysJustPressed.insert(key);
            }
            m_keysDown.insert(key);
        } break;

        case sf::Event::KeyReleased: {
            sf::Keyboard::Key key = event.key.code;
            // Remove from keysDown, mark “just released”.
            m_keysDown.erase(key);
            m_keysJustReleased.insert(key);
        } break;

                                   /// -- Mouse button events --
        case sf::Event::MouseButtonPressed: {
            sf::Mouse::Button btn = event.mouseButton.button;
            if (m_mouseDown.find(btn) == m_mouseDown.end()) {
                m_mouseJustPressed.insert(btn);
            }
            m_mouseDown.insert(btn);
        } break;

        case sf::Event::MouseButtonReleased: {
            sf::Mouse::Button btn = event.mouseButton.button;
            m_mouseDown.erase(btn);
            m_mouseJustReleased.insert(btn);
        } break;

                                           /// -- Mouse move event --
        case sf::Event::MouseMoved: {
            m_mousePosition.x = event.mouseMove.x;
            m_mousePosition.y = event.mouseMove.y;
        } break;

                                  /// -- Mouse wheel scrolled --
        case sf::Event::MouseWheelScrolled: {
            // event.mouseWheelScroll.delta is a float, but usually +1 or -1 per notch.
            m_mouseWheelDelta += static_cast<int>(event.mouseWheelScroll.delta);
        } break;

        default:
            // Other events (resized, text entered, etc.) we ignore here.
            break;
        }
    }

    // 4.3) If no MouseMoved event arrived this frame, update position via SFML directly:
    //       (This ensures m_mousePosition is valid even if the mouse didn't move in this frame.)
    if (m_mousePosition == sf::Vector2i(0, 0)) {
        m_mousePosition = sf::Mouse::getPosition(window);
    }
}

///-------------------------------------------
/// 5) Keyboard Query Methods
///-------------------------------------------
bool InputService::isKeyDown(sf::Keyboard::Key key) const {
    return (m_keysDown.find(key) != m_keysDown.end());
}

bool InputService::wasKeyPressed(sf::Keyboard::Key key) const {
    return (m_keysJustPressed.find(key) != m_keysJustPressed.end());
}

bool InputService::wasKeyReleased(sf::Keyboard::Key key) const {
    return (m_keysJustReleased.find(key) != m_keysJustReleased.end());
}

///-------------------------------------------
/// 6) Mouse Query Methods
///-------------------------------------------
bool InputService::isMouseButtonDown(sf::Mouse::Button button) const {
    return (m_mouseDown.find(button) != m_mouseDown.end());
}

bool InputService::wasMouseButtonPressed(sf::Mouse::Button button) const {
    return (m_mouseJustPressed.find(button) != m_mouseJustPressed.end());
}

bool InputService::wasMouseButtonReleased(sf::Mouse::Button button) const {
    return (m_mouseJustReleased.find(button) != m_mouseJustReleased.end());
}

sf::Vector2i InputService::getMousePosition() const {
    return m_mousePosition;
}

int InputService::getMouseWheelDelta() const {
    return m_mouseWheelDelta;
}

#include "../../include/UI/MenuEventHandler.h"
#include "../../include/UI/MenuButtonManager.h"
#include "../../include/Core/SmartExceptions.h"
#include <iostream>

using namespace SmartExceptions;

MenuEventHandler::MenuEventHandler() {
    // Register UI recovery strategies
    RecoveryHandler::instance().registerRecoveryStrategy(Category::UI, []() -> bool {
        // Try to reset UI state
        try {
            // Any UI reset operations would go here
            return true;
        } catch (...) {
            return false;
        }
    });
    
    std::cout << "MenuEventHandler: Initialized for event processing with smart exception system" << std::endl;
}

bool MenuEventHandler::handleEvents(sf::RenderWindow& window, MenuButtonManager& buttonManager) {
    return SAFE_EXECUTE_WITH_RETURN(
        [&]() -> bool {
            if (!m_enabled) {
                return false;
            }

            sf::Event event;
            while (window.pollEvent(event)) {
                if (!isValidEvent(event)) {
                    THROW_INPUT_EXCEPTION(
                        "Invalid event received in menu event handler",
                        Severity::Warning,
                        RecoveryStrategy::Skip
                    );
                }

                // Handle different event types
                if (handleWindowEvents(window, event)) {
                    return true; // Window should close
                }

                if (handleKeyboardEvents(window, event)) {
                    return true; // Exit requested
                }

                handleMouseEvents(buttonManager, event);
            }

            return false; // Continue running
        },
        "handleEvents",
        "MenuEventHandler",
        Category::Input
    ).value_or(false);
}

bool MenuEventHandler::handleWindowEvents(sf::RenderWindow& window, const sf::Event& event) {
    return SAFE_EXECUTE_WITH_RETURN(
        [&]() -> bool {
            if (event.type == sf::Event::Closed) {
                std::cout << "MenuEventHandler: Window close requested" << std::endl;
                window.close();
                return true;
            }
            return false;
        },
        "handleWindowEvents",
        "MenuEventHandler",
        Category::UI
    ).value_or(false);
}

bool MenuEventHandler::handleKeyboardEvents(sf::RenderWindow& window, const sf::Event& event) {
    return SAFE_EXECUTE_WITH_RETURN(
        [&]() -> bool {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    std::cout << "MenuEventHandler: Escape key pressed - closing window" << std::endl;
                    window.close();
                    return true;
                }
            }
            return false;
        },
        "handleKeyboardEvents",
        "MenuEventHandler",
        Category::Input
    ).value_or(false);
}

bool MenuEventHandler::handleMouseEvents(MenuButtonManager& buttonManager, const sf::Event& event) {
    return SAFE_EXECUTE_WITH_RETURN(
        [&]() -> bool {
            bool handled = false;

            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2f mousePos(
                    static_cast<float>(event.mouseMove.x),
                    static_cast<float>(event.mouseMove.y)
                );
                
                if (mousePos.x < 0 || mousePos.y < 0) {
                    THROW_INPUT_EXCEPTION(
                        "Invalid mouse position: (" + std::to_string(mousePos.x) + ", " + std::to_string(mousePos.y) + ")",
                        Severity::Warning,
                        RecoveryStrategy::Skip
                    );
                }
                
                buttonManager.handleButtonHover(mousePos);
                handled = true;
            }
            else if (event.type == sf::Event::MouseButtonPressed &&
                     event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(
                    static_cast<float>(event.mouseButton.x),
                    static_cast<float>(event.mouseButton.y)
                );
                
                if (mousePos.x < 0 || mousePos.y < 0) {
                    THROW_INPUT_EXCEPTION(
                        "Invalid mouse click position: (" + std::to_string(mousePos.x) + ", " + std::to_string(mousePos.y) + ")",
                        Severity::Warning,
                        RecoveryStrategy::Skip
                    );
                }
                
                handled = buttonManager.handleButtonClick(mousePos);
            }

            return handled;
        },
        "handleMouseEvents",
        "MenuEventHandler",
        Category::Input
    ).value_or(false);
}

bool MenuEventHandler::isValidEvent(const sf::Event& event) const {
    return SAFE_EXECUTE_WITH_RETURN(
        [&]() -> bool {
            // Basic event validation
            if (event.type < sf::Event::Closed || event.type > sf::Event::Count) {
                return false;
            }
            
            // Add more specific validation as needed
            return true;
        },
        "isValidEvent",
        "MenuEventHandler",
        Category::Input
    ).value_or(false);
}
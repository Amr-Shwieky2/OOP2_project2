#include "MenuScreen.h"
#include "Logger.h"
#include "ScreenTypes.h" // Defines the enum values like SCREEN_GAME, SCREEN_HELP, etc.

MenuScreen::MenuScreen() {
    try {
        // Load background image
        sf::Texture& bg = AppContext::instance().resources().getTexture("menu_background.png");
        m_background.setTexture(bg);

        // Load font
        m_font = AppContext::instance().resources().getFont("arial.ttf");

        // Create menu items
        float startY = 200.0f;
        float spacing = 60.0f;

        for (const auto& label : m_labels) {
            sf::Text text;
            text.setFont(m_font);
            text.setString(label);
            text.setCharacterSize(30);
            text.setFillColor(sf::Color::White);
            text.setPosition(100.0f, startY);

            m_menuItems.push_back(text);
            m_itemBounds.push_back(text.getGlobalBounds());

            startY += spacing;
        }

        Logger::log("MenuScreen initialized");
    }
    catch (const std::exception& e) {
        Logger::log(std::string("MenuScreen init failed: ") + e.what(), LogLevel::Error);
    }
}

void MenuScreen::handleEvents(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::MouseMoved) {
            updateHoverEffect(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
        }

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            handleClick(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
        }
    }
}

void MenuScreen::update(float deltaTime) {
    // Could animate title or items if desired
}

void MenuScreen::render(sf::RenderWindow& window) {
    window.draw(m_background);
    for (const auto& text : m_menuItems) {
        window.draw(text);
    }
}

void MenuScreen::updateHoverEffect(const sf::Vector2i& mousePos) {
    for (std::size_t i = 0; i < m_menuItems.size(); ++i) {
        if (m_itemBounds[i].contains(static_cast<sf::Vector2f>(mousePos))) {
            m_menuItems[i].setFillColor(sf::Color::Yellow);
        }
        else {
            m_menuItems[i].setFillColor(sf::Color::White);
        }
    }
}

void MenuScreen::handleClick(const sf::Vector2i& mousePos) {
    for (std::size_t i = 0; i < m_menuItems.size(); ++i) {
        if (m_itemBounds[i].contains(static_cast<sf::Vector2f>(mousePos))) {
            Logger::log("Menu item clicked: " + m_labels[i]);
            AppContext::instance().audio().playSound("click");

            ScreenManager& manager = AppContext::instance().screenManager(); // Assuming screenManager accessor

            if (m_labels[i] == "Play") {
                manager.changeScreen(ScreenType::Game);
            }
            else if (m_labels[i] == "Help") {
                manager.changeScreen(ScreenType::Help);
            }
            else if (m_labels[i] == "Settings") {
                manager.changeScreen(ScreenType::Settings);
            }
            else if (m_labels[i] == "Exit") {
                std::exit(0);
            }
        }
    }
}

#include "PauseScreen.h"
#include "AppContext.h"
#include "Logger.h"
#include "ScreenTypes.h"

PauseScreen::PauseScreen() {
    try {
        m_font = AppContext::instance().resources().getFont("arial.ttf");

        m_title.setFont(m_font);
        m_title.setString("Game Paused");
        m_title.setCharacterSize(40);
        m_title.setFillColor(sf::Color::White);
        m_title.setPosition(100.f, 50.f);

        float startY = 150.f;
        float spacing = 50.f;

        for (const auto& label : m_labels) {
            sf::Text option;
            option.setFont(m_font);
            option.setString(label);
            option.setCharacterSize(28);
            option.setFillColor(sf::Color::White);
            option.setPosition(100.f, startY);

            m_options.push_back(option);
            m_optionBounds.push_back(option.getGlobalBounds());
            startY += spacing;
        }

        Logger::log("PauseScreen initialized");

    }
    catch (const std::exception& e) {
        Logger::log(std::string("PauseScreen init failed: ") + e.what(), LogLevel::Error);
    }
}

void PauseScreen::handleEvents(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::MouseMoved)
            updateHover(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            handleClick(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
    }
}

void PauseScreen::update(float) {
    // Optional animations or timer logic
}

void PauseScreen::render(sf::RenderWindow& window) {
    window.draw(m_title);
    for (const auto& option : m_options)
        window.draw(option);
}

void PauseScreen::updateHover(const sf::Vector2i& mousePos) {
    for (std::size_t i = 0; i < m_options.size(); ++i) {
        if (m_optionBounds[i].contains(static_cast<sf::Vector2f>(mousePos)))
            m_options[i].setFillColor(sf::Color::Yellow);
        else
            m_options[i].setFillColor(sf::Color::White);
    }
}

void PauseScreen::handleClick(const sf::Vector2i& mousePos) {
    for (std::size_t i = 0; i < m_options.size(); ++i) {
        if (m_optionBounds[i].contains(static_cast<sf::Vector2f>(mousePos))) {
            AppContext::instance().audio().playSound("click");
            Logger::log("Pause option selected: " + m_labels[i]);

            if (m_labels[i] == "Resume") {
                AppContext::instance().screenManager().changeScreen(ScreenType::Game);
            }
            else if (m_labels[i] == "Settings") {
                AppContext::instance().screenManager().changeScreen(ScreenType::Settings);
            }
            else if (m_labels[i] == "Exit to Menu") {
                AppContext::instance().screenManager().changeScreen(ScreenType::Menu);
            }
        }
    }
}

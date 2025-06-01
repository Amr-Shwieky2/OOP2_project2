#include "SettingsScreen.h"
#include "AppContext.h"
#include "Logger.h"
#include "ScreenTypes.h"

SettingsScreen::SettingsScreen() {
    try {
        m_font = AppContext::instance().resources().getFont("arial.ttf");

        m_title.setFont(m_font);
        m_title.setString("Settings");
        m_title.setCharacterSize(40);
        m_title.setFillColor(sf::Color::White);
        m_title.setPosition(100.f, 50.f);

        m_musicLabel.setFont(m_font);
        m_musicLabel.setString("Music Volume");
        m_musicLabel.setCharacterSize(24);
        m_musicLabel.setFillColor(sf::Color::White);
        m_musicLabel.setPosition(100.f, 150.f);

        m_effectsLabel.setFont(m_font);
        m_effectsLabel.setString("Effects Volume");
        m_effectsLabel.setCharacterSize(24);
        m_effectsLabel.setFillColor(sf::Color::White);
        m_effectsLabel.setPosition(100.f, 250.f);

        m_musicSlider.setSize({ 200.f, 10.f });
        m_musicSlider.setFillColor(sf::Color::Green);
        m_musicSlider.setPosition(300.f, 165.f);

        m_effectsSlider.setSize({ 200.f, 10.f });
        m_effectsSlider.setFillColor(sf::Color::Blue);
        m_effectsSlider.setPosition(300.f, 265.f);

        m_backButton.setFont(m_font);
        m_backButton.setString("Back");
        m_backButton.setCharacterSize(28);
        m_backButton.setFillColor(sf::Color::White);
        m_backButton.setPosition(100.f, 350.f);
        m_backBounds = m_backButton.getGlobalBounds();

        Logger::log("SettingsScreen initialized");

    }
    catch (const std::exception& e) {
        Logger::log(std::string("SettingsScreen init failed: ") + e.what(), LogLevel::Error);
    }
}

void SettingsScreen::handleEvents(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::MouseMoved)
            updateHover(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            handleClick(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

        if (event.type == sf::Event::MouseButtonReleased) {
            m_draggingMusic = false;
            m_draggingEffects = false;
        }

        if (event.type == sf::Event::MouseMoved && (m_draggingMusic || m_draggingEffects)) {
            handleSliderDrag(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
        }
    }
}

void SettingsScreen::update(float) {
    // Update volume based on slider length
    float musicPercent = (m_musicSlider.getSize().x - 0.01f) / 200.f;
    float effectsPercent = (m_effectsSlider.getSize().x - 0.01f) / 200.f;

    m_musicVolume = musicPercent * 100.f;
    m_effectsVolume = effectsPercent * 100.f;

    AppContext::instance().audio().setMusicVolume(m_musicVolume);
    AppContext::instance().audio().setEffectsVolume(m_effectsVolume);
}

void SettingsScreen::render(sf::RenderWindow& window) {
    window.draw(m_title);
    window.draw(m_musicLabel);
    window.draw(m_musicSlider);
    window.draw(m_effectsLabel);
    window.draw(m_effectsSlider);
    window.draw(m_backButton);
}

void SettingsScreen::updateHover(const sf::Vector2i& pos) {
    if (m_backBounds.contains(static_cast<sf::Vector2f>(pos))) {
        m_backButton.setFillColor(sf::Color::Yellow);
    }
    else {
        m_backButton.setFillColor(sf::Color::White);
    }
}

void SettingsScreen::handleClick(const sf::Vector2i& pos) {
    if (m_backBounds.contains(static_cast<sf::Vector2f>(pos))) {
        AppContext::instance().audio().playSound("click");
        AppContext::instance().screenManager().changeScreen(ScreenType::Menu);
        return;
    }

    sf::FloatRect musicBounds = m_musicSlider.getGlobalBounds();
    if (musicBounds.contains(static_cast<sf::Vector2f>(pos))) {
        m_draggingMusic = true;
        handleSliderDrag(pos);
    }

    sf::FloatRect effectsBounds = m_effectsSlider.getGlobalBounds();
    if (effectsBounds.contains(static_cast<sf::Vector2f>(pos))) {
        m_draggingEffects = true;
        handleSliderDrag(pos);
    }
}

void SettingsScreen::handleSliderDrag(const sf::Vector2i& pos) {
    float newX = static_cast<float>(pos.x);
    float baseX = m_musicSlider.getPosition().x;
    float maxWidth = 200.f;

    if (m_draggingMusic) {
        float length = std::clamp(newX - baseX, 0.f, maxWidth);
        m_musicSlider.setSize({ length, 10.f });
    }

    if (m_draggingEffects) {
        float length = std::clamp(newX - baseX, 0.f, maxWidth);
        m_effectsSlider.setSize({ length, 10.f });
    }
}

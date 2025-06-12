#include "../../include/Screens/MenuScreen.h"
#include "AppContext.h"
#include "ScreenTypes.h"
#include "../UI/ObservableButton.h"
#include "../UI/MenuButtonObserver.h"

MenuScreen::MenuScreen() {
    // Load background image or create fallback gradient
    try {
        m_backgroundTexture = AppContext::instance().resources().getTexture("MenuScreen.png");
        m_backgroundSprite.setTexture(m_backgroundTexture);

        // Scale to fit 1600x900 window
        sf::Vector2u textureSize = m_backgroundTexture.getSize();
        sf::Vector2f targetSize(1600.0f, 900.0f);
        float scaleX = targetSize.x / textureSize.x;
        float scaleY = targetSize.y / textureSize.y;
        m_backgroundSprite.setScale(scaleX, scaleY);
    }
    catch (...) {
        // Fallback: create gradient background
        m_backgroundTexture.create(1600, 900);
        sf::Uint8* pixels = new sf::Uint8[1600 * 900 * 4];
        for (int y = 0; y < 900; y++) {
            for (int x = 0; x < 1600; x++) {
                int index = (y * 1600 + x) * 4;
                float gradient = (float)y / 900.0f;
                pixels[index] = (sf::Uint8)(50 + 100 * gradient);     // R
                pixels[index + 1] = (sf::Uint8)(30 + 80 * gradient);  // G
                pixels[index + 2] = (sf::Uint8)(80 + 120 * gradient); // B
                pixels[index + 3] = 255; // A
            }
        }
        m_backgroundTexture.update(pixels);
        m_backgroundSprite.setTexture(m_backgroundTexture);
        delete[] pixels;
    }

    setupButtons();
}

void MenuScreen::setupButtons() {
    std::cout << "Setting up buttons with Factory Pattern..." << std::endl;

    // Create observer for button events
    m_buttonObserver = std::make_shared<MenuButtonObserver>();

    // Button layout configuration
    const float buttonWidth = 300.0f;
    const float buttonHeight = 100.0f;
    const float buttonSpacing = 150.0f;
    const float startY = 200.0f;
    const float centerX = (1400.0f - buttonWidth - 120.0f);

    const float aboutButtonWidth = 130.0f;
    const float aboutButtonHeight = 100.0f;
    const float aboutButtonX = 30.0f;
    const float aboutButtonY = 650.0f;

    // Clear existing buttons
    m_observableButtons.clear();

    // Create buttons using Factory Pattern for consistency
    std::cout << "Creating buttons using Factory Pattern..." << std::endl;

    // About Us button
    auto aboutBtn = ButtonFactory::createAboutButton(
        sf::Vector2f(aboutButtonX, aboutButtonY),
        sf::Vector2f(aboutButtonWidth, aboutButtonHeight),
        m_buttonObserver,
        m_font
    );
    m_observableButtons.push_back(std::move(aboutBtn));

    // Start Game button
    auto startBtn = ButtonFactory::createStartButton(
        sf::Vector2f(centerX, startY),
        sf::Vector2f(buttonWidth, buttonHeight),
        m_buttonObserver,
        m_font
    );
    m_observableButtons.push_back(std::move(startBtn));

    // Settings button
    auto settingsBtn = ButtonFactory::createSettingsButton(
        sf::Vector2f(centerX, startY + buttonSpacing),
        sf::Vector2f(buttonWidth, buttonHeight),
        m_buttonObserver,
        m_font
    );
    m_observableButtons.push_back(std::move(settingsBtn));

    // Help button
    auto helpBtn = ButtonFactory::createHelpButton(
        sf::Vector2f(centerX, startY + 2 * buttonSpacing),
        sf::Vector2f(buttonWidth, buttonHeight),
        m_buttonObserver,
        m_font
    );
    m_observableButtons.push_back(std::move(helpBtn));

    // Exit button
    auto exitBtn = ButtonFactory::createExitButton(
        sf::Vector2f(centerX, startY + 3 * buttonSpacing),
        sf::Vector2f(buttonWidth, buttonHeight),
        m_buttonObserver,
        m_font
    );
    m_observableButtons.push_back(std::move(exitBtn));

    std::cout << "Created " << m_observableButtons.size() << " buttons using Factory Pattern!" << std::endl;
}

void MenuScreen::handleEvents(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            window.close();
        }

        // Observer Pattern in action - handle button interactions
        if (event.type == sf::Event::MouseMoved) {
            sf::Vector2f mousePos(static_cast<float>(event.mouseMove.x),
                static_cast<float>(event.mouseMove.y));

            for (auto& button : m_observableButtons) {
                button->handleMouseMove(mousePos);  // Sends hover notifications
            }
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x),
                    static_cast<float>(event.mouseButton.y));

                for (auto& button : m_observableButtons) {
                    if (button->handleClick(mousePos)) {  // Sends click notifications
                        break; // Stop after first button clicked
                    }
                }
            }
        }
    }
}

void MenuScreen::update(float deltaTime) {
    m_animationTime += deltaTime;

    // Animate title text with glow effect
    float glowIntensity = 0.7f + 0.3f * sin(m_animationTime * 2.0f);
    sf::Color titleColor = sf::Color::Yellow;
    titleColor.a = (sf::Uint8)(255 * glowIntensity);
    m_titleText.setFillColor(titleColor);

    // Update all observable buttons
    for (auto& button : m_observableButtons) {
        button->update(deltaTime);
    }
}

void MenuScreen::render(sf::RenderWindow& window) {
    // Draw background
    window.draw(m_backgroundSprite);

    // Draw title with shadow effect
    sf::Text titleShadow = m_titleText;
    titleShadow.setFillColor(sf::Color(0, 0, 0, 100));
    titleShadow.setPosition(m_titleText.getPosition().x + 5, m_titleText.getPosition().y + 5);
    window.draw(titleShadow);
    window.draw(m_titleText);

    // Draw all observable buttons
    for (auto& button : m_observableButtons) {
        button->render(window);
    }
}

void MenuScreen::updateSelection(int direction) {
    // Removed for simplicity - using mouse-only navigation
}

void MenuScreen::selectCurrentButton() {
    // Removed for simplicity - mouse clicks handle everything
}
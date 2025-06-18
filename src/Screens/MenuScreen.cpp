#include "../../include/Screens/MenuScreen.h"
#include "AppContext.h"
#include "ScreenTypes.h"

MenuScreen::MenuScreen() {
    // Load background image
    try {
        m_backgroundTexture = AppContext::instance().resources().getTexture("MenuScreen.png");
        m_backgroundSprite.setTexture(m_backgroundTexture);

        // Scale background to fit window (1600x900)
        sf::Vector2u textureSize = m_backgroundTexture.getSize();
        sf::Vector2f targetSize(1600.0f, 900.0f);

        float scaleX = targetSize.x / textureSize.x;
        float scaleY = targetSize.y / textureSize.y;
        m_backgroundSprite.setScale(scaleX, scaleY);
    }
    catch (...) {
        // Fallback: create a gradient background for 1600x900
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
    auto& resources = AppContext::instance().resources();

    const float buttonWidth = 300.0f;
    const float buttonHeight = 100.0f;
    const float buttonSpacing = 150.0f;
    const float startY = 200.0f;
    const float centerX = (1400.0f - buttonWidth - 120.0f);

    struct ButtonData {
        std::string text;
        std::string imagePath;
        sf::Color fallbackColor;
    };

    std::vector<ButtonData> buttonData = {
        {"START GAME", "StartButtonEnglish.png", sf::Color(80, 200, 80, 255)},
        {"SETTINGS", "SettingsButtonEnglish.png", sf::Color(80, 80, 200, 255)},
        {"HELP", "HelpButtonEnglish.png", sf::Color(200, 200, 80, 255)},
        {"EXIT", "ExitButtonEnglish.png", sf::Color(200, 80, 80, 255)}
    };

    m_buttons.clear();
    m_buttons.reserve(buttonData.size());

    for (size_t i = 0; i < buttonData.size(); i++) {
        sf::Vector2f position(centerX, startY + i * buttonSpacing);
        sf::Vector2f size(buttonWidth, buttonHeight);

        Button button(position, size, buttonData[i].text);
        button.setFont(m_font);

        // Try to load image
        try {
            sf::Texture& texture = resources.getTexture(buttonData[i].imagePath);
            button.setButtonImage(&texture);
            button.setTextColor(sf::Color::Transparent); // Hide text since image has text
        }
        catch (...) {
            // Use colored rectangle fallback
            button.setBackgroundColor(buttonData[i].fallbackColor);
            button.setTextColor(sf::Color::White);
        }

        m_buttons.push_back(button);
    }

    handleButtonCallbacks();
}

void MenuScreen::handleButtonCallbacks() {
    auto& screenManager = AppContext::instance().screenManager();

    if (m_buttons.size() >= 4) {
        m_buttons[0].setCallback([&screenManager]() {
            screenManager.changeScreen(ScreenType::PLAY);
            });

        m_buttons[1].setCallback([&screenManager]() {
            screenManager.changeScreen(ScreenType::SETTINGS);
            });

        m_buttons[2].setCallback([&screenManager]() {
            screenManager.changeScreen(ScreenType::HELP);
            });

        m_buttons[3].setCallback([]() {
            exit(0);
            });
    }
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

        // Handle mouse movement for hover effects
        if (event.type == sf::Event::MouseMoved) {
            sf::Vector2f mousePos(event.mouseMove.x, event.mouseMove.y);
            for (auto& button : m_buttons) {
                button.handleMouseMove(mousePos);
            }
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

                for (auto& button : m_buttons) {
                    if (button.handleClick(mousePos)) {
                        break; // Button was clicked, stop checking others
                    }
                }
            }
        }
    }
}

void MenuScreen::update(float deltaTime) {
    m_animationTime += deltaTime;

    // Animate title (gentle glow effect)
    float glowIntensity = 0.7f + 0.3f * sin(m_animationTime * 2.0f);
    sf::Color titleColor = sf::Color::Yellow;
    titleColor.a = (sf::Uint8)(255 * glowIntensity);
    m_titleText.setFillColor(titleColor);

    // Update button animations
    for (auto& button : m_buttons) {
        button.update(deltaTime);
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

    // Draw all buttons
    for (auto& button : m_buttons) {
        button.render(window);
    }
}

void MenuScreen::updateSelection(int direction) {
    // Not needed with simple button class
    // Keyboard navigation removed for simplicity
}

void MenuScreen::selectCurrentButton() {
    // Not needed with simple button class  
    // Mouse clicks handle everything now
}
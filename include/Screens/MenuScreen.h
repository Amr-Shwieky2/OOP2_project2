#pragma once

#include "../Core/IScreen.h"
#include "../UI/ObservableButton.h"
#include "../UI/MenuButtonObserver.h"
#include "../UI/Button.h"
#include "../UI/ButtonFactory.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <Button.h>

class MenuScreen : public IScreen {
public:
    MenuScreen();
    ~MenuScreen() = default;

    // IScreen interface implementation
    void handleEvents(sf::RenderWindow& window) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    // Font for text rendering
    sf::Font m_font;
    sf::Text m_titleText;

    // Background image
    sf::Texture m_backgroundTexture;
    sf::Sprite m_backgroundSprite;

    // UI Elements - Buttons
    std::vector<Button> m_buttons;

    // Animation and effects
    float m_animationTime = 0.0f;

    std::vector<std::unique_ptr<ObservableButton>> m_observableButtons;
    std::shared_ptr<MenuButtonObserver> m_buttonObserver;

    // Private helper methods
    void setupButtons();
    void updateSelection(int direction);
    void selectCurrentButton();
};
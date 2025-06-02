#pragma once

#include "IScreen.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

/**
 * MainMenuScreen shows:
 *    • A title at the top
 *    • Two menu items: “Play” and “Exit”
 *
 * When “Play” is clicked (or Enter is pressed), we switch to GameplayScreen (handled by caller).
 * When “Exit” is clicked (or Escape is pressed), we close the window.
 */
class MainMenuScreen : public IScreen {
public:
    MainMenuScreen();
    virtual ~MainMenuScreen() = default;

    // IScreen interface:
    void handleInput(sf::RenderWindow& window) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void onEnter() override;
    void onExit() override;

    // Expose which button was chosen (optional getter for external logic)
    enum class Selection {
        None,
        Play,
        Exit
    };
    Selection getSelection() const;

private:
    void buildMenu();         // create texts/shapes
    int  getItemUnderMouse(sf::Vector2i mousePos) const;
    void selectItem(int idx);

    sf::Font       m_font;            // Loaded via ResourceLoader in constructor
    sf::Text       m_titleText;

    std::vector<sf::Text>      m_menuTexts;   // “Play” and “Exit”
    std::vector<sf::RectangleShape> m_menuBacks; // semi‐transparent rect behind each text

    Selection      m_selected;       // currently clicked/pressed
    int            m_hoveredIndex;   // index currently hovered by mouse, -1 if none
};

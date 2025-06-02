#include "MainMenuScreen.h"
#include "ResourceLoader.h"
#include "AudioService.h"
#include "InputService.h"
#include "Logger.h"
#include <iostream>

// Constants for positioning
static const float TITLE_Y = 100.f;
static const float MENU_START_Y = 250.f;
static const float MENU_SPACING = 60.f;

//-------------------------------------------------
// Constructor
//-------------------------------------------------
MainMenuScreen::MainMenuScreen()
    : m_selected(Selection::None),
    m_hoveredIndex(-1)
{
    // 1) Load the font via ResourceLoader (ensure “mainFont” was loaded earlier)
    try {
        m_font = ResourceLoader::instance().getFont("mainFont");
    }
    catch (const std::exception& e) {
        Logger::instance().logError("MainMenuScreen: failed to get font 'mainFont': " + std::string(e.what()));
        // As fallback, try a default font file directly:
        if (!m_font.loadFromFile("resources/fonts/arial.ttf")) {
            std::cerr << "MainMenuScreen: fallback font load failed.\n";
        }
    }

    // 2) Build title text
    m_titleText.setFont(m_font);
    m_titleText.setString("Desert Ball");
    m_titleText.setCharacterSize(48);
    m_titleText.setFillColor(sf::Color::White);

    // Center the title horizontally
    sf::FloatRect titleBounds = m_titleText.getLocalBounds();
    m_titleText.setOrigin(titleBounds.width / 2.f, titleBounds.height / 2.f);
    // We’ll center at window midpoint in render() since window size may be unknown here

    // 3) Build menu items (“Play” and “Exit”)
    buildMenu();
}

//-------------------------------------------------
// buildMenu()
//-------------------------------------------------
void MainMenuScreen::buildMenu() {
    const std::vector<std::string> labels = { "Play", "Exit" };

    for (size_t i = 0; i < labels.size(); ++i) {
        sf::Text txt;
        txt.setFont(m_font);
        txt.setString(labels[i]);
        txt.setCharacterSize(32);
        txt.setFillColor(sf::Color::White);

        // Center origin on each text
        sf::FloatRect bounds = txt.getLocalBounds();
        txt.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

        m_menuTexts.push_back(txt);

        // Background rectangle (semi‐transparent) behind each text
        sf::RectangleShape rect;
        rect.setSize(sf::Vector2f(bounds.width + 40.f, bounds.height + 20.f));
        rect.setFillColor(sf::Color(0, 0, 0, 150));
        rect.setOrigin(rect.getSize().x / 2.f, rect.getSize().y / 2.f);
        m_menuBacks.push_back(rect);
    }
}

//-------------------------------------------------
// onEnter()
// Called once when this screen becomes active
//-------------------------------------------------
void MainMenuScreen::onEnter() {
    Logger::instance().logInfo("Entered MainMenuScreen");
}

//-------------------------------------------------
// onExit()
// Called once when this screen is popped or replaced
//-------------------------------------------------
void MainMenuScreen::onExit() {
    Logger::instance().logInfo("Exiting MainMenuScreen");
}

//-------------------------------------------------
// handleInput()
// Polls InputService to detect clicks or keypresses
//-------------------------------------------------
void MainMenuScreen::handleInput(sf::RenderWindow& window) {
    // Update hoveredIndex by checking mouse position against each menu item
    sf::Vector2i mousePos = InputService::instance().getMousePosition();
    int newHover = getItemUnderMouse(mousePos);
    if (newHover != m_hoveredIndex) {
        m_hoveredIndex = newHover;
        // Optionally play a hover sound:
        // AudioService::instance().playSound("hover");
    }

    // If mouse clicked (left button)
    if (InputService::instance().wasMouseButtonPressed(sf::Mouse::Left)) {
        if (m_hoveredIndex >= 0) {
            selectItem(m_hoveredIndex);
        }
    }

    // “Enter” key selects current hovered index
    if (InputService::instance().wasKeyPressed(sf::Keyboard::Enter) && (m_hoveredIndex >= 0)) {
        selectItem(m_hoveredIndex);
    }

    // “Escape” or down/up arrow navigation
    if (InputService::instance().wasKeyPressed(sf::Keyboard::Escape)) {
        // Treat Escape as Exit
        m_selected = Selection::Exit;
    }
    if (InputService::instance().wasKeyPressed(sf::Keyboard::Down) ||
        InputService::instance().wasKeyPressed(sf::Keyboard::Up))
    {
        // If nothing hovered yet, start on first item
        if (m_hoveredIndex < 0) {
            m_hoveredIndex = 0;
        }
        else {
            // Cycle through items
            if (InputService::instance().wasKeyPressed(sf::Keyboard::Down)) {
                m_hoveredIndex = (m_hoveredIndex + 1) % static_cast<int>(m_menuTexts.size());
            }
            else {
                m_hoveredIndex = (m_hoveredIndex - 1 + static_cast<int>(m_menuTexts.size()))
                    % static_cast<int>(m_menuTexts.size());
            }
        }
        // Play “hover” sound if you like
        // AudioService::instance().playSound("hover");
    }
}

//-------------------------------------------------
// update()
// For MainMenu, no continuous animation needed—empty
//-------------------------------------------------
void MainMenuScreen::update(float /*deltaTime*/) {
    // Nothing to update per frame in this static menu
}

//-------------------------------------------------
// render()
// Draw title + menu items, highlighting hovered item
//-------------------------------------------------
void MainMenuScreen::render(sf::RenderWindow& window) {
    sf::Vector2u winSize = window.getSize();
    float centerX = static_cast<float>(winSize.x) / 2.f;

    // 1) Draw background (if desired)
    // window.clear(sf::Color(50, 50, 80));

    // 2) Draw title, centered horizontally
    m_titleText.setPosition(centerX, TITLE_Y);
    window.draw(m_titleText);

    // 3) Draw each menu item
    for (size_t i = 0; i < m_menuTexts.size(); ++i) {
        float posY = MENU_START_Y + i * MENU_SPACING;

        // Update text position
        m_menuTexts[i].setPosition(centerX, posY);

        // Update background rectangle position & color
        m_menuBacks[i].setPosition(centerX, posY);
        if (static_cast<int>(i) == m_hoveredIndex) {
            m_menuBacks[i].setFillColor(sf::Color(100, 100, 200, 200)); // highlight
            m_menuTexts[i].setFillColor(sf::Color::Yellow);
        }
        else {
            m_menuBacks[i].setFillColor(sf::Color(0, 0, 0, 150));
            m_menuTexts[i].setFillColor(sf::Color::White);
        }

        window.draw(m_menuBacks[i]);
        window.draw(m_menuTexts[i]);
    }
}

//-------------------------------------------------
// getItemUnderMouse()
// Returns the index of the menu item if mouse is over its rect, otherwise -1
//-------------------------------------------------
int MainMenuScreen::getItemUnderMouse(sf::Vector2i mousePos) const {
    for (size_t i = 0; i < m_menuBacks.size(); ++i) {
        // getGlobalBounds expects top-left corner, so convert shape origin to absolute coords
        sf::FloatRect bounds = m_menuBacks[i].getGlobalBounds();
        if (bounds.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

//-------------------------------------------------
// selectItem()
// Called when user clicks or presses Enter on an item
//-------------------------------------------------
void MainMenuScreen::selectItem(int idx) {
    switch (idx) {
    case 0:
        m_selected = Selection::Play;
        AudioService::instance().playSound("click");
        break;
    case 1:
        m_selected = Selection::Exit;
        AudioService::instance().playSound("click");
        break;
    default:
        m_selected = Selection::None;
        break;
    }
}

//-------------------------------------------------
// getSelection()
// Returns what the user chose; caller can inspect this after update()
//-------------------------------------------------
MainMenuScreen::Selection MainMenuScreen::getSelection() const {
    return m_selected;
}

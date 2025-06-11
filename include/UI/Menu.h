#pragma once

#include "Widget.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <functional>

///
/// Menu
/// ----
/// A simple vertical list of text items. The user can navigate up/down with arrow keys
/// or hover with the mouse, and “activate” (click/press Enter) the currently highlighted item.
///
class Menu : public Widget {
public:
    Menu();
    ~Menu() override;

    // Add a menu entry (text + callback to invoke on selection)
    void addEntry(const std::string& text, const std::function<void()>& callback);

    // Set the font (all entries use the same font)
    void setFont(const sf::Font& font);

    // Set character size for each entry
    void setCharacterSize(unsigned int size);

    // Handle keyboard/mouse events to move selection or activate
    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;

    // Update (e.g., highlight on mouse hover)
    void update(float deltaTime) override;

    // Draw all menu entries
    void render(sf::RenderTarget& target) override;

private:
    struct Entry {
        sf::Text text;
        std::function<void()> callback;
    };

    std::vector<Entry> m_entries;
    int m_selectedIndex = -1;  // -1 = no selection yet

    // Helper: highlight only the selected entry
    void updateHighlight();
};

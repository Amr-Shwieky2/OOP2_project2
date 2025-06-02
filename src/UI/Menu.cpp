#include "Menu.h"

Menu::Menu() {
}

Menu::~Menu() {
}

void Menu::addEntry(const std::string& str, const std::function<void()>& callback) {
    Entry e;
    e.text.setString(str);
    e.callback = callback;
    // The font and character size should be set by user beforehand:
    // caller should have done setFont(...) and setCharacterSize(...)
    // e.text.setFont(m_font); e.text.setCharacterSize(m_charSize);

    // Position will be determined in update()
    m_entries.push_back(std::move(e));

    // If first entry, select it by default
    if (m_selectedIndex < 0) {
        m_selectedIndex = 0;
    }
}

void Menu::setFont(const sf::Font& font) {
    for (auto& e : m_entries) {
        e.text.setFont(font);
    }
}

void Menu::setCharacterSize(unsigned int size) {
    for (auto& e : m_entries) {
        e.text.setCharacterSize(size);
    }
}

void Menu::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (!m_visible || !m_enabled || m_entries.empty()) return;

    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up) {
            m_selectedIndex = (m_selectedIndex - 1 + static_cast<int>(m_entries.size())) % static_cast<int>(m_entries.size());
            updateHighlight();
        }
        else if (event.key.code == sf::Keyboard::Down) {
            m_selectedIndex = (m_selectedIndex + 1) % static_cast<int>(m_entries.size());
            updateHighlight();
        }
        else if (event.key.code == sf::Keyboard::Enter) {
            if (m_selectedIndex >= 0) {
                m_entries[m_selectedIndex].callback();
            }
        }
    }
    else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        // Check if click inside any entry’s bounds
        sf::Vector2f mp(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
        for (size_t i = 0; i < m_entries.size(); ++i) {
            if (m_entries[i].text.getGlobalBounds().contains(mp)) {
                m_selectedIndex = static_cast<int>(i);
                updateHighlight();
                m_entries[i].callback();
                break;
            }
        }
    }
}

void Menu::update(float deltaTime) {
    (void)deltaTime;

    if (!m_visible) return;

    // Re‐compute positions of each entry (stacked vertically under m_position)
    float yOffset = 0.f;
    for (size_t i = 0; i < m_entries.size(); ++i) {
        m_entries[i].text.setPosition(m_position.x, m_position.y + yOffset);
        yOffset += m_entries[i].text.getLocalBounds().height + 10.f;
    }

    // Highlight selected entry
    updateHighlight();
}

void Menu::render(sf::RenderTarget& target) {
    if (!m_visible) return;
    for (auto& e : m_entries) {
        target.draw(e.text);
    }
}

void Menu::updateHighlight() {
    for (size_t i = 0; i < m_entries.size(); ++i) {
        if (static_cast<int>(i) == m_selectedIndex) {
            m_entries[i].text.setFillColor(sf::Color::Yellow);
        }
        else {
            m_entries[i].text.setFillColor(sf::Color::White);
        }
    }
}

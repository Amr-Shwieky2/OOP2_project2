#include "UIManager.h"
#include <algorithm>

UIManager::UIManager() {
}

UIManager::~UIManager() {
    m_widgets.clear();
}

void UIManager::addWidget(std::shared_ptr<Widget> widget) {
    m_widgets.push_back(std::move(widget));
}

void UIManager::removeWidget(std::shared_ptr<Widget> widget) {
    auto it = std::find(m_widgets.begin(), m_widgets.end(), widget);
    if (it != m_widgets.end()) {
        m_widgets.erase(it);
    }
}

void UIManager::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    for (auto& w : m_widgets) {
        if (w->isVisible() && w->isEnabled()) {
            w->handleEvent(event, window);
        }
    }
}

void UIManager::update(float deltaTime) {
    for (auto& w : m_widgets) {
        if (w->isVisible()) {
            w->update(deltaTime);
        }
    }
}

void UIManager::render(sf::RenderTarget& target) {
    for (auto& w : m_widgets) {
        if (w->isVisible()) {
            w->render(target);
        }
    }
}

#include "Widget.h"

Widget::Widget()
    : m_position(0.f, 0.f),
    m_size(100.f, 30.f),
    m_visible(true),
    m_enabled(true)
{
}

Widget::~Widget() {
}

void Widget::setPosition(const sf::Vector2f& pos) {
    m_position = pos;
}

sf::Vector2f Widget::getPosition() const {
    return m_position;
}

void Widget::setSize(const sf::Vector2f& s) {
    m_size = s;
}

sf::Vector2f Widget::getSize() const {
    return m_size;
}

void Widget::setVisible(bool v) {
    m_visible = v;
}

bool Widget::isVisible() const {
    return m_visible;
}

void Widget::setEnabled(bool e) {
    m_enabled = e;
}

bool Widget::isEnabled() const {
    return m_enabled;
}

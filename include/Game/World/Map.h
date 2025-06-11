#pragma once

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <vector>

class Map {
public:
    Map(b2World& world);

    void render(sf::RenderWindow& window);

private:
    std::vector<b2Body*> m_bodies;
    std::vector<sf::RectangleShape> m_shapes;

    void addPlatform(b2World& world, const sf::Vector2f& pos, const sf::Vector2f& size);
};

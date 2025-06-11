#pragma once

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

class Ball {
public:
    Ball(b2World& world, const sf::Vector2f& position);

    void moveLeft();
    void moveRight();
    void jump();

    void update(float dt);
    void render(sf::RenderWindow& window);

private:
    b2Body* m_body;
    sf::CircleShape m_shape;

    bool m_canJump = false; // simple jump check
};

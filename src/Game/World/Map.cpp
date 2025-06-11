#include "Map.h"

constexpr float SCALE = 30.f;

Map::Map(b2World& world) {
    // Add ground
    addPlatform(world, { 400.f, 580.f }, { 800.f, 40.f });

    // Add a floating platform
    addPlatform(world, { 300.f, 400.f }, { 200.f, 20.f });
}

void Map::addPlatform(b2World& world, const sf::Vector2f& pos, const sf::Vector2f& size) {
    b2BodyDef bodyDef;
    bodyDef.position.Set(pos.x / SCALE, pos.y / SCALE);

    b2Body* body = world.CreateBody(&bodyDef);

    b2PolygonShape box;
    box.SetAsBox((size.x / 2.f) / SCALE, (size.y / 2.f) / SCALE);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;
    fixtureDef.density = 0.f;
    fixtureDef.friction = 0.5f;

    body->CreateFixture(&fixtureDef);
    m_bodies.push_back(body);

    sf::RectangleShape shape(size);
    shape.setOrigin(size.x / 2.f, size.y / 2.f);
    shape.setPosition(pos);
    shape.setFillColor(sf::Color(100, 100, 100));
    m_shapes.push_back(shape);
}

void Map::render(sf::RenderWindow& window) {
    for (const auto& shape : m_shapes)
        window.draw(shape);
}

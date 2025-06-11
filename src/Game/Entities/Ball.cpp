#include "Ball.h"

constexpr float BALL_RADIUS = 20.f;
constexpr float BALL_SPEED = 4.f;
constexpr float JUMP_FORCE = -10.f;

Ball::Ball(b2World& world, const sf::Vector2f& position) {
    // Define body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(position.x / 30.f, position.y / 30.f); // Box2D in meters
    m_body = world.CreateBody(&bodyDef);

    // Define shape
    b2CircleShape shape;
    shape.m_radius = BALL_RADIUS / 30.f;

    // Define fixture
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    m_body->CreateFixture(&fixtureDef);

    // Setup SFML shape
    m_shape.setRadius(BALL_RADIUS);
    m_shape.setOrigin(BALL_RADIUS, BALL_RADIUS);
    m_shape.setFillColor(sf::Color::Red);
}

void Ball::moveLeft() {
    m_body->SetLinearVelocity(b2Vec2(-BALL_SPEED, m_body->GetLinearVelocity().y));
}

void Ball::moveRight() {
    m_body->SetLinearVelocity(b2Vec2(BALL_SPEED, m_body->GetLinearVelocity().y));
}

void Ball::jump() {
    if (m_canJump) {
        m_body->ApplyLinearImpulseToCenter(b2Vec2(0.f, JUMP_FORCE), true);
        m_canJump = false;
    }
}

void Ball::update(float dt) {
    // rudimentary ground detection
    if (std::abs(m_body->GetLinearVelocity().y) < 0.05f)
        m_canJump = true;

    b2Vec2 pos = m_body->GetPosition();
    m_shape.setPosition(pos.x * 30.f, pos.y * 30.f);
}

void Ball::render(sf::RenderWindow& window) {
    window.draw(m_shape);
}

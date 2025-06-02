#pragma once

#include "GameObject.h"
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

// Forward‐declare AppContext so we can access the physics world and resource loader
class AppContext;

///
/// Ball
/// ----
/// The player’s ball. Uses Box2D for physics. In update(), it syncs its SFML sprite
/// to the Box2D body’s position (with a scale factor). onCollision() can be expanded
/// later to handle interactions with other GameObjects.
///
class Ball : public GameObject {
public:
    // Constructor: needs AppContext so we can get the b2World and ResourceLoader
    Ball(AppContext& context, const b2Vec2& spawnPosition);

    // Destructor: default is fine (PhysicsComponent will be cleaned up by GameObject base)
    ~Ball() override = default;

    // Update the ball each frame: sync sprite position to physics body
    void update(float deltaTime) override;

    // Draw the ball’s sprite to the window
    void render(sf::RenderWindow& window) override;

    // Called when this ball collides with another GameObject
    void onCollision(GameObject& other) override;

    // Applies a jump impulse if the ball is (roughly) on the ground
    void jump();

    // Applies a horizontal force to move left/right
    void moveLeft();
    void moveRight();

private:
    AppContext& m_context;
    sf::Texture     m_texture;
    sf::Sprite      m_sprite;

    static constexpr float PIXELS_PER_METER = 100.0f;
    // If Box2D is using meters, we multiply by this to convert to pixels.

    // Fixture properties
    static constexpr float RADIUS_METERS = 0.5f; // 0.5 meter radius
    static constexpr float DENSITY = 1.0f;
    static constexpr float FRICTION = 0.3f;
    static constexpr float RESTITUTION = 0.4f;
};

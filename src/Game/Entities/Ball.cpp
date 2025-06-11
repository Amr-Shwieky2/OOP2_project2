//#include "Ball.h"
//#include "AppContext.h"
//#include "PhysicsComponent.h"
//#include "ResourceLoader.h"
//
//#include <iostream>  // for debug prints
//
//Ball::Ball(AppContext& context, const b2Vec2& spawnPosition)
//    : m_context(context)
//{
//    // 1) Load the texture via ResourceLoader so we don’t repeatedly load from disk.
//    //    We assume ResourceLoader stores textures with the key "images/entities/ball.png"
//    try {
//        m_texture = m_context.getResourceLoader().getTexture("images/entities/ball.png");
//    }
//    catch (const std::exception& e) {
//        std::cerr << "Failed to load ball texture: " << e.what() << "\n";
//    }
//
//    // 2) Create and configure the SFML sprite
//    m_sprite.setTexture(m_texture);
//
//    // Center the sprite’s origin so it rotates around its center:
//    sf::Vector2u texSize = m_texture.getSize();
//    m_sprite.setOrigin(texSize.x / 2.0f, texSize.y / 2.0f);
//
//    // 3) Create a Box2D body definition for a dynamic circle (radius = RADIUS_METERS)
//    b2BodyDef bodyDef;
//    bodyDef.type = b2_dynamicBody;
//    bodyDef.position = spawnPosition;  // spawnPosition is in Box2D world units (meters)
//    bodyDef.fixedRotation = false;      // allow rotation
//    bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(this);
//    // (Optional: store pointer to this Ball for collision callbacks)
//
//    // 4) Instantiate the PhysicsComponent, which creates the b2Body in the Box2D world
//    auto* world = &m_context.getPhysicsWorld();
//    auto physicsComp = std::make_unique<PhysicsComponent>(*world, bodyDef);
//
//    // 5) Add a circular fixture to the body
//    b2CircleShape circleShape;
//    circleShape.m_radius = RADIUS_METERS;
//
//    b2FixtureDef fixtureDef;
//    fixtureDef.shape = &circleShape;
//    fixtureDef.density = DENSITY;
//    fixtureDef.friction = FRICTION;
//    fixtureDef.restitution = RESTITUTION;
//
//    physicsComp->getBody()->CreateFixture(&fixtureDef);
//
//    // 6) Attach the PhysicsComponent to this GameObject
//    setPhysicsComponent(std::move(physicsComp));
//}
//
//void Ball::update(float deltaTime) {
//    // 1) Retrieve Box2D body position (in meters)
//    auto* phys = getPhysicsComponent();
//    if (!phys) return;
//
//    b2Vec2 posMeters = phys->getPosition();
//
//    // 2) Convert from Box2D meters to SFML pixels
//    float xPixels = posMeters.x * PIXELS_PER_METER;
//    float yPixels = (posMeters.y * PIXELS_PER_METER);
//
//    // Box2D uses y-up, SFML uses y-down. We must flip Y:
//    // If your “ground” is at y=0 in Box2D, your window’s ground (pixels) might be at the bottom:
//    //
//    //    SFML y = windowHeight - (Box2D y * PIXELS_PER_METER)
//    //
//    // We assume the ground (Box2D y=0) is at the bottom of the SFML window (pixel height).
//    float windowHeight = static_cast<float>(m_context.getScreenManager().getActiveScreen()->getWindow().getSize().y);
//    // Note: Above is just a sketch. If you can’t fetch window from screen, store windowHeight elsewhere.
//
//    float flippedYPixels = windowHeight - yPixels;
//
//    // 3) Update the SFML sprite’s position
//    m_sprite.setPosition(xPixels, flippedYPixels);
//}
//
//void Ball::render(sf::RenderWindow& window) {
//    window.draw(m_sprite);
//}
//
//void Ball::onCollision(GameObject& other) {
//    // This is called when Box2D reports a collision between this ball’s body and 'other'.
//    // At this stage, you might play a sound or deduct health, etc.
//    // Example debug print:
//    // std::cout << "Ball collided with another object!\n";
//}
//
//void Ball::jump() {
//    // Apply an upward impulse/force if ball is roughly on the ground (y ≤ some threshold)
//    auto* phys = getPhysicsComponent();
//    if (!phys) return;
//
//    b2Vec2 velocity = phys->getBody()->GetLinearVelocity();
//    if (std::fabs(velocity.y) < 0.01f) {
//        // Only jump if nearly stationary vertically
//        b2Vec2 impulse(0.0f, 5.0f);
//        phys->getBody()->ApplyLinearImpulseToCenter(impulse, true);
//    }
//}
//
//void Ball::moveLeft() {
//    auto* phys = getPhysicsComponent();
//    if (!phys) return;
//
//    b2Vec2 vel = phys->getBody()->GetLinearVelocity();
//    vel.x = -3.0f; // 3 m/s to the left
//    phys->getBody()->SetLinearVelocity(vel);
//}
//
//void Ball::moveRight() {
//    auto* phys = getPhysicsComponent();
//    if (!phys) return;
//
//    b2Vec2 vel = phys->getBody()->GetLinearVelocity();
//    vel.x = 3.0f; // 3 m/s to the right
//    phys->getBody()->SetLinearVelocity(vel);
//}

#pragma once

#include <box2d/box2d.h>
#include <memory>

///
/// PhysicsService
/// --------------
/// Owns a single Box2D world. Steps the world forward each frame.
/// Provides access to the world for entity and component creation.
///
class PhysicsService {
public:
    PhysicsService();
    ~PhysicsService();

    // Step the physics world by 'timeStep' seconds, with velocity/position iterations.
    void step(float timeStep, int velocityIterations = 8, int positionIterations = 3);

    // Retrieve the raw Box2D world so PhysicsComponent can create bodies.
    b2World& getWorld();

private:
    // A single Box2D world with downward gravity of −9.8 m/s² in the y‐direction.
    std::unique_ptr<b2World> m_world;
};

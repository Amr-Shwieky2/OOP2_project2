#include "PhysicsService.h"

PhysicsService::PhysicsService() {
    // Create a world with standard gravity pointing downward
    m_world = std::make_unique<b2World>(b2Vec2(0.0f, -9.8f));
}

PhysicsService::~PhysicsService() {
    // The unique_ptr will delete the world automatically.
}

void PhysicsService::step(float timeStep, int velocityIterations, int positionIterations) {
    if (m_world) {
        m_world->Step(timeStep, velocityIterations, positionIterations);
    }
}

b2World& PhysicsService::getWorld() {
    return *m_world;
}

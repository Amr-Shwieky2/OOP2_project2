#include "GameplayScreen.h"

constexpr float TIME_STEP = 1.f / 60.f;
constexpr int VELOCITY_ITER = 8;
constexpr int POSITION_ITER = 3;

GameplayScreen::GameplayScreen(AppContext& context)
    : m_context(context),
    m_world({ 0.f, 9.8f }),  // gravity
    m_player(m_world, { 100.f, 100.f }),
    m_map(m_world)
{
}

void GameplayScreen::handleEvent(sf::Event& event) {
    m_player.handleInput(event);
}

void GameplayScreen::update(float dt) {
    m_world.Step(TIME_STEP, VELOCITY_ITER, POSITION_ITER);
    m_player.update(dt);
}

void GameplayScreen::render(sf::RenderWindow& window) {
    m_map.render(window);
    m_player.render(window);
}

#pragma once

#include "IScreen.h"
#include "AppContext.h"
#include "Ball.h"
#include "Map.h"

#include <box2d/box2d.h>
#include <memory>

class GameplayScreen : public IScreen {
public:
    GameplayScreen(AppContext& context);

    void handleEvent(sf::Event& event) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    AppContext& m_context;
    b2World m_world;

    Ball m_player;
    Map m_map;
};

#pragma once  
#include "../Core/IScreen.h"  
#include <SFML/Graphics.hpp>  

class GameplayScreen : public IScreen {  
public:  
   GameplayScreen();  

   void handleEvents(sf::RenderWindow& window) override;  
   void update(float deltaTime) override;  
   void render(sf::RenderWindow& window) override;  

private:  
   sf::Font m_font;  
   sf::Text m_gameText;  
   sf::CircleShape m_player;  
   sf::Vector2f m_playerVelocity;  
   bool m_gameRunning = true;  
};

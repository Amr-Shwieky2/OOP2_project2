#pragma once  
#include "../Core/IScreen.h"  
#include <SFML/Graphics.hpp>  

class HelpScreen : public IScreen {  
public:  
   HelpScreen();  

   void handleEvents(sf::RenderWindow& window) override;  
   void update(float deltaTime) override;  
   void render(sf::RenderWindow& window) override;  

private:  
   sf::Font m_font;  
   sf::Text m_titleText;  
   sf::Text m_helpText;  
   sf::Text m_backText;  
};
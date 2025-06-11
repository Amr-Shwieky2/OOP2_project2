#pragma once  
#include "../Core/IRenderable.h"  
#include "../Core/IUpdatable.h"  
#include "../Core/IEventHandler.h"  
#include <SFML/Graphics.hpp>  

class UIComponent : public IRenderable, public IUpdatable, public IEventHandler {  
public:  
   UIComponent(const sf::Vector2f& position = sf::Vector2f(0, 0),  
       const sf::Vector2f& size = sf::Vector2f(100, 50));  
   virtual ~UIComponent() = default;  

   // Position and Size  
   virtual void setPosition(const sf::Vector2f& position);  
   virtual void setSize(const sf::Vector2f& size);  
   virtual sf::Vector2f getPosition() const { return m_position; }  
   virtual sf::Vector2f getSize() const { return m_size; }  
   virtual sf::FloatRect getBounds() const;  

   // Visibility and State  
   virtual void setVisible(bool visible) { m_visible = visible; }  
   virtual bool isVisible() const { return m_visible; }  
   virtual bool contains(const sf::Vector2f& point) const;  

protected:  
   sf::Vector2f m_position;  
   sf::Vector2f m_size;  
   bool m_visible = true;  
};
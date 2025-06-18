#pragma once  
#include "../Core/ICommand.h"  
#include "../Config/ScreenTypes.h"  
#include <string>  

/**  
* @brief Command لمعالجة مفتاح ESC مع دعم الـ Undo  
* يحفظ الشاشة الحالية ويذهب للهدف (عادة MENU)  
*/  
class EscapeKeyCommand : public ICommand {  
public:  
   // Constructor  
   EscapeKeyCommand(ScreenType currentScreen, ScreenType targetScreen = ScreenType::MENU);  

   // ICommand interface  
   void execute() override;  
   void undo() override;  
   bool canUndo() const override;  
   std::string getName() const override;  

private:  
   ScreenType m_currentScreen;     // الشاشة التي نخرج منها  
   ScreenType m_targetScreen;      // الشاشة المستهدفة (عادة MENU)  
   bool m_hasExecuted = false;     // هل تم تنفيذ الأمر  

   // Helper function  
   std::string screenTypeToString(ScreenType screen) const;  
};
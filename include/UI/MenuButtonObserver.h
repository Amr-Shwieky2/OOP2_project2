#pragma once

#include "IButtonObserver.h"
#include "../Core/CommandInvoker.h"
#include "../Commands/ChangeScreenCommand.h"
#include "../Commands/ExitGameCommand.h"
#include "../Config/ScreenTypes.h"
#include <iostream>
#include <memory>

// Observer للأزرار مع Command Pattern
class MenuButtonObserver : public IButtonObserver {
public:
    MenuButtonObserver();
    ~MenuButtonObserver() = default;

    // IButtonObserver interface
    void onButtonClicked(const std::string& buttonId) override;
    void onButtonHovered(const std::string& buttonId) override;

    // إدارة الأوامر
    CommandInvoker& getCommandInvoker() { return m_commandInvoker; }

    // دوال مساعدة للـ Undo/Redo (للاستخدام المستقبلي)
    bool undoLastAction();
    bool redoLastAction();
    void printCommandHistory();

private:
    CommandInvoker m_commandInvoker;  // مدير الأوامر

    // Command Pattern handlers
    void handleStartButton();
    void handleSettingsButton();
    void handleHelpButton();
    void handleAboutButton();
    void handleExitButton();

    // دالة مساعدة لتحديد الشاشة الحالية
    ScreenType getCurrentScreen() const;
};
file (GLOB_RECURSE MY_SOURCE_FILES CONFIGURE_DEPENDS LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_LIST_DIR} *.cpp)
target_sources (${CMAKE_PROJECT_NAME} PRIVATE ${MY_SOURCE_FILES} "Core/IScreen.cpp"  "UI/Button.cpp" "Screens/MenuScreen.cpp"  "Screens/SettingsScreen.cpp" "../include/Screens/LoadingScreen.h" "Screens/LoadingScreen.cpp" "../include/Screens/HelpScreen.h" "Screens/HelpScreen.cpp" "../include/Core/LanguageManager.h"  "Core/AudioManager.cpp" "../include/Core/AudioManager.h" "../include/UI/Slider.h" "UI/Slider.cpp" "UI/Dropdown.cpp" "../include/UI/Dropdown.h" "../include/Core/UITheme.h" "Core/UITheme.cpp"  "../include/UI/SettingsUIFactory.h" "../include/Core/ResourceManager.h" "../include/Core/MultiFontManager.h"    "Screens/AboutScreen.cpp" "../include/UI/VolumeControlPanel.h" "UI/VolumeControlPanel.cpp" "../include/UI/LanguageControlPanel.h" "UI/LanguageControlPanel.cpp" "UI/SettingsEventHandler.cpp" "../include/UI/SettingsEventHandler.h" "../include/Core/SimpleObserver.h" "../include/Core/IButtonObserver.h" "../include/Core/ButtonSubject.h" "../include/UI/ObservableButton.h" "../include/UI/MenuButtonObserver.h" "../include/UI/ButtonFactory.h" "UI/ButtonFactory.cpp" "../include/Core/ICommand.h" "../include/Core/CommandInvoker.h" "Core/CommandInvoker.cpp"  "Commands/ChangeScreenCommand.cpp" "../include/Commands/ExitGameCommand.h" "UI/MenuButtonObserver.cpp""Commands/ExitGameCommand.cpp" "../include/Commands/EscapeKeyCommand.h")


add_subdirectory(Core)
add_subdirectory(UI)
add_subdirectory(Services)
add_subdirectory(Application)

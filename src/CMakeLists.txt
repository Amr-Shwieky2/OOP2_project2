file (GLOB_RECURSE MY_SOURCE_FILES CONFIGURE_DEPENDS LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_LIST_DIR} *.cpp)
<<<<<<< HEAD
target_sources (${CMAKE_PROJECT_NAME} PRIVATE ${MY_SOURCE_FILES} "Core/IScreen.cpp" "Core/State.cpp" "Services/InputService.cpp" "Services/RandomService.cpp" "Services/PhysicsService.cpp" "UI/Button.cpp" "UI/Label.cpp" "UI/HUD.cpp" "UI/Menu.cpp" "UI/UIManager.cpp" "UI/Widget.cpp" "Screens/SplashScreen.cpp" "Screens/MenuScreen.cpp" "Screens/GameplayScreen.cpp" "Screens/PauseScreen.cpp" "Screens/SettingsScreen.cpp" "Screens/GameOverScreen.cpp" "Screens/VictoryScreen.cpp" "Screens/TutorialScreen.cpp" "Screens/CreditsScreen.cpp" "Screens/CharacterSelectScreen.cpp" "../include/Screens/LoadingScreen.h" "Screens/LoadingScreen.cpp" "../include/Screens/HelpScreen.h" "Screens/HelpScreen.cpp"  "../include/UI/UIComponent.h" "../include/Core/LanguageManager.h"  "Core/AudioManager.cpp" "../include/Core/AudioManager.h" "../include/UI/Slider.h" "UI/Slider.cpp" "UI/Dropdown.cpp" "../include/UI/Dropdown.h" "../include/Core/UITheme.h" "Core/UITheme.cpp"  "../include/UI/SettingsUIFactory.h" "../include/Core/ResourceManager.h" "../include/Core/StateManager.h"  "../include/Core/MultiFontManager.h"    "Screens/AboutScreen.cpp")
=======
target_sources (${CMAKE_PROJECT_NAME} PRIVATE ${MY_SOURCE_FILES} "../include/Game/Entities/Entity.h" "Game/Entities/Entity.cpp" "../include/Game/Components/Component.h" "Game/Components/Component.cpp")
>>>>>>> 70f8dadc4c8b82d7171402a7e9888286f78eafbe


add_subdirectory(Core)
add_subdirectory(UI)
add_subdirectory(Services)
add_subdirectory(Application)

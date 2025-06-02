file (GLOB_RECURSE MY_SOURCE_FILES CONFIGURE_DEPENDS LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_LIST_DIR} *.cpp)
target_sources (${CMAKE_PROJECT_NAME} PRIVATE ${MY_SOURCE_FILES} "../include/Game/Entities/Entity.h" "Game/Entities/Entity.cpp" "../include/Game/Components/Component.h" "Game/Components/Component.cpp")


add_subdirectory(Core)
add_subdirectory(UI)
add_subdirectory(Services)
add_subdirectory(Application)

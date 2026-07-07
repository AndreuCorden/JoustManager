cmake_minimum_required(VERSION 3.16)
project(JoustManager LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# 1. Set a default build type (Release) if none is explicitly provided.
# This is useful for single-configuration generators like MinGW Makefiles or Ninja.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "No build type specified. Defaulting to 'Release'.")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Points to your Qt binaries
set(CMAKE_PREFIX_PATH "C:/Qt/6.11.1/mingw_64") 

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# Explicitly mapping out every single file so CMake cannot skip them
set(SOURCES
    main.cpp
    resources.qrc
    src/Player.h
    src/Player.cpp
    src/GameTimelineController.h
    src/GameTimelineController.cpp
    src/GameWindow.h
    src/GameWindow.cpp
    src/GameDialog.h
    src/GameDialog.cpp
    src/MainMenu.h
    src/MainMenu.cpp
    src/ManagerInterface.h
    src/ManagerInterface.cpp
    src/model/knights/Knight.h
    src/model/knights/Knight.cpp
    src/model/knights/KnightRosterTabHandler.h
    src/model/knights/KnightRosterTabHandler.cpp
    src/model/knights/KnightRecruitmentTabHandler.h
    src/model/knights/KnightRecruitmentTabHandler.cpp
    src/view/knights/KnightDetailDialog.h
    src/view/knights/KnightDetailDialog.cpp
    src/view/knights/KnightRosterView.h
    src/view/knights/KnightRosterView.cpp
    src/view/knights/KnightRecruitmentView.h
    src/view/knights/KnightRecruitmentView.cpp
    src/controller/knights/KnightRosterTabController.h
    src/controller/knights/KnightRosterTabController.cpp
    src/controller/knights/KnightRecruitmentTabController.h
    src/controller/knights/KnightRecruitmentTabController.cpp
    src/model/items/Item.h
    src/model/items/Item.cpp
    src/model/items/ShopTabHandler.h
    src/model/items/ShopTabHandler.cpp
    src/model/items/Armour/GothicArmour.h
    src/model/items/Armour/GothicArmour.cpp
    src/model/items/Weapon/CourteousLance.h
    src/model/items/Weapon/CourteousLance.cpp
    src/view/items/ShopView.h
    src/view/items/ShopView.cpp
    src/view/items/ItemDetailDialog.h
    src/view/items/ItemDetailDialog.cpp
    src/controller/items/ShopTabController.h
    src/controller/items/ShopTabController.cpp
    src/model/tournaments/Tournament.h
    src/model/tournaments/Tournament.cpp
    src/model/tournaments/TournamentTabHandler.h
    src/model/tournaments/TournamentTabHandler.cpp
    src/view/tournaments/TournamentRunnerDialog.h
    src/view/tournaments/TournamentRunnerDialog.cpp
    src/view/tournaments/KnightSelectionDialog.h
    src/view/tournaments/KnightSelectionDialog.cpp
    src/view/tournaments/TournamentView.h
    src/view/tournaments/TournamentView.cpp
    src/controller/tournaments/TournamentTabController.h
    src/controller/tournaments/TournamentTabController.cpp
    src/Tournaments/Joust/JoustView.h
    src/Tournaments/Joust/JoustView.cpp
)

# Compile the executable using the explicit list
add_executable(JoustManager ${SOURCES})

target_link_libraries(JoustManager PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)
target_include_directories(JoustManager PRIVATE src)
cmake_minimum_required(VERSION 3.16)
project(JoustManager LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

# Points to your Qt binaries
set(CMAKE_PREFIX_PATH "C:/Qt/6.11.1/mingw_64") 

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# Explicitly mapping out every single file so CMake cannot skip them
set(SOURCES
    main.cpp
    src/Player.h
    src/Player.cpp
    src/GameWindow.h
    src/GameWindow.cpp
    src/GameDialog.h
    src/GameDialog.cpp
    src/MainMenu.h
    src/MainMenu.cpp
    src/ManagerInterface.h
    src/ManagerInterface.cpp
    src/Knights/KnightRosterTab.h
    src/Knights/KnightRosterTab.cpp
    src/Knights/Knight.h
    src/Knights/Knight.cpp
    src/Knights/KnightDetailDialog.h
    src/Knights/KnightDetailDialog.cpp
    src/Knights/KnightRecruitmentTab.h
    src/Knights/KnightRecruitmentTab.cpp
    src/Items/Item.h
    src/Items/Item.cpp
    src/Items/Armour/GothicArmour.h
    src/Items/Armour/GothicArmour.cpp
    src/Items/Weapon/CourteousLance.h
    src/Items/Weapon/CourteousLance.cpp
)

# Compile the executable using the explicit list
add_executable(JoustManager ${SOURCES})

target_link_libraries(JoustManager PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)
target_include_directories(JoustManager PRIVATE src)
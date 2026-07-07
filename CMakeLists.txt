cmake_minimum_required(VERSION 3.16)
project(JoustManager LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

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
    src/Knights/KnightRosterTab.h
    src/Knights/KnightRosterTab.cpp
    src/Knights/Knight.h
    src/Knights/Knight.cpp
    src/Knights/KnightDetailDialog.h
    src/Knights/KnightDetailDialog.cpp
    src/Knights/KnightRecruitmentTab.h
    src/Knights/KnightRecruitmentTab.cpp
    src/model/items/Item.h
    src/model/items/Item.cpp
    src/model/items/ShopTabHandler.h
    src/model/items/ShopTabHandler.cpp
    src/model/items/Armour/GothicArmour.h
    src/model/items/Armour/GothicArmour.cpp
    src/model/items/Weapon/CourteousLance.h
    src/model/items/Weapon/CourteousLance.cpp
    src/controller/items/ShopTabController.h
    src/controller/items/ShopTabController.cpp
    src/view/items/ShopView.h
    src/view/items/ShopView.cpp
    src/view/items/ItemDetailDialog.h
    src/view/items/ItemDetailDialog.cpp
    src/Tournaments/TournamentTab.h
    src/Tournaments/TournamentTab.cpp
    src/Tournaments/Tournament.h
    src/Tournaments/Tournament.cpp
    src/Tournaments/TournamentRunnerDialog.h
    src/Tournaments/TournamentRunnerDialog.cpp
    src/Tournaments/KnightSelectionDialog.h
    src/Tournaments/KnightSelectionDialog.cpp
    src/Tournaments/Joust/JoustView.h
    src/Tournaments/Joust/JoustView.cpp
)

# Compile the executable using the explicit list
add_executable(JoustManager ${SOURCES})

target_link_libraries(JoustManager PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)
target_include_directories(JoustManager PRIVATE src)
cmake_minimum_required(VERSION 3.16)
project(JoustManager LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

# Point directly to your newly installed MinGW version folder!
set(CMAKE_PREFIX_PATH "C:/Qt/6.11.1/mingw_64") 

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

file(GLOB SOURCES "src/*.cpp" "src/*.h")
add_executable(JoustManager main.cpp ${SOURCES})

target_link_libraries(JoustManager PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)
cmake_minimum_required(VERSION 3.0.0)
project(game-engine VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CTest)
enable_testing()

add_library(game-engine src/game-engine.cpp src/Engine.hpp src/Engine.cpp src/Engine/Error.hpp src/Engine/Error.cpp src/Engine/Color.hpp)

find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-image CONFIG REQUIRED)
find_package(sdl2-gfx CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC SDL2::SDL2main SDL2::SDL2-static SDL2::SDL2_gfx SDL2::SDL2_image)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

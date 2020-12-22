cmake_minimum_required(VERSION 3.0.0)
project(game-engine VERSION 0.1.0)

include(CTest)
enable_testing()

add_library(game-engine src/game-engine.cpp src/Engine.hpp src/Engine.cpp)

find_package(SDL2 REQUIRED COMPONENTS image gfx)
target_link_libraries(${PROJECT_NAME} SDL2 SDL2main SDL2_image SDL2_gfx)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

cmake_minimum_required(VERSION 3.13)

project(EngineSandbox VERSION 1.0
                      DESCRIPTION "Game engine developement sandbox"
                      LANGUAGES CXX)

add_subdirectory(engine)

add_executable(lostInCave main.cpp Player.cpp Wall.cpp Game.cpp Hazard.cpp Gem.cpp Background.cpp MenuButton.cpp)

target_include_directories(lostInCave PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(lostInCave engine)
cmake_minimum_required(VERSION 3.20)

project(SnakeGame3)

set(CMAKE_CXX_STANDARD 17)

add_executable(SnakeGame3 main.cpp)

add_subdirectory(C:/raylib/raylib raylib_build)

target_link_libraries(SnakeGame3 PRIVATE raylib)
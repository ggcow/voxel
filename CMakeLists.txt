cmake_minimum_required(VERSION 3.17)
project(opengl C)

set(CMAKE_C_STANDARD 99)

file(GLOB_RECURSE SOURCES src/*.c)

add_executable(opengl ${SOURCES})

target_link_libraries(${PROJECT_NAME} m GL pthread SDL2)
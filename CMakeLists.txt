cmake_minimum_required(VERSION 3.17)
project(opengl C)

set(CMAKE_C_STANDARD 99)


include_directories(include)
add_executable(opengl src/core/common.c src/core/control.c src/cube.c src/draw.c src/core/event.c src/core/log.c src/main.c src/map.c src/matrix.c src/player.c src/core/renderer.c src/core/shader.c src/core/timer.c src/core/window.c)

set(GCC_COMPILE_FLAGS "-Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COMPILE_FLAGS}")

target_link_libraries(${PROJECT_NAME} m GL pthread SDL2)
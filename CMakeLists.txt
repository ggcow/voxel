cmake_minimum_required(VERSION 3.17)
project(opengl C)

set(CMAKE_C_STANDARD 99)

add_definitions(-DROOT_FOLDER=\"${CMAKE_SOURCE_DIR}\")

include_directories(include)
add_executable(opengl src/common.c src/control.c src/cube.c src/event.c src/log.c src/main.c src/matrix.c src/player.c src/renderer.c src/shader.c src/timer.c src/window.c src/chunk.c include/chunk.h src/buffer.c src/map.c include/map.h src/list.c include/list.h)

set(GCC_COMPILE_FLAGS "-Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COMPILE_FLAGS}")

target_link_libraries(${PROJECT_NAME} m GL pthread SDL2)
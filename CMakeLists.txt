cmake_minimum_required(VERSION 3.8)
project(chip8 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SOURCE_DIR "src")
file(GLOB SOURCES "src/*.c")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${project_SOURCE_DIR}/cmake")
find_package(SDL2 REQUIRED)
add_executable(chip8 ${SOURCES})


include_directories(${SDL2_INCLUDE_DIR})
target_link_libraries(chip8 ${SDL2_LIBRARY})
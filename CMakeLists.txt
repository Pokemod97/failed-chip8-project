cmake_minimum_required(VERSION 3.7)
project(chip8 C)

set(CMAKE_C_STANDARD 11)

#set(SOURCE_DIR "src")
file(GLOB SOURCES src/*.c)
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${project_SOURCE_DIR}/cmake")
find_package(SDL2 REQUIRED)
include_directories(SDL2)

add_executable(chip8 ${SOURCES})
target_link_libraries(chip8 SDL2)


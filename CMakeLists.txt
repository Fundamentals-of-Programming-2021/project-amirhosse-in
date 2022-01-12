cmake_minimum_required(VERSION 3.15)
project(project_amirhosse_in C)

set(CMAKE_C_STANDARD 11)

add_executable(project_amirhosse_in main.c)

include_directories("/usr/include/SDL2")
target_link_libraries(project_amirhosse_in m SDL2 SDL2_gfx)
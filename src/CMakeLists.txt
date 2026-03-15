cmake_minimum_required(VERSION 3.20)
project(PROJECTOR)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(util)
add_subdirectory(vulkan)
add_subdirectory(core)
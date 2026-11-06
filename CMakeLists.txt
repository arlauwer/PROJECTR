cmake_minimum_required(VERSION 3.20)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(IncludeRoots)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Choose clang compiler
if(NOT DEFINED CMAKE_CXX_COMPILER AND NOT DEFINED ENV{CXX})
    find_program(CLANGXX clang++)
    if(CLANGXX)
        set(CMAKE_CXX_COMPILER "${CLANGXX}" CACHE FILEPATH "")
        set(CMAKE_C_COMPILER clang CACHE FILEPATH "")
    endif()
endif()

# Create symbolic link to compile_commands
file(CREATE_LINK
    ${CMAKE_BINARY_DIR}/compile_commands.json
    ${CMAKE_SOURCE_DIR}/compile_commands.json
    SYMBOLIC)

project(PROJECTR)

# Default to an optimised build
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

enable_testing()

add_include_root(src      ${CMAKE_SOURCE_DIR}/src)
add_include_root(external ${CMAKE_SOURCE_DIR}/external SYSTEM) # SYSTEM hides third-party warnings

add_subdirectory(external)
add_subdirectory(src)
add_subdirectory(test)

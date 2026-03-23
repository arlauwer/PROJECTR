set(TARGET main)

add_executable(${TARGET} main.cpp)

target_link_libraries(${TARGET} util vulkan core)
target_include_directories(${TARGET} PRIVATE ../util ../vulkan ../core)
set(TARGET core)

file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.hpp")

add_library(${TARGET} STATIC ${SOURCES} ${HEADERS})

target_link_libraries(${TARGET} util vulkan)
target_include_directories(${TARGET} PRIVATE ../util ../vulkan)
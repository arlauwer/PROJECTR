set(TARGET vulkan)

find_package(Vulkan REQUIRED)

file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.hpp")

add_library(${TARGET} STATIC ${SOURCES} ${HEADERS})

target_link_libraries(${TARGET} Vulkan::Vulkan util)
target_include_directories(${TARGET} PRIVATE ../util)


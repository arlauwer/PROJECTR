set(TARGET util)

file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.hpp")

add_library(${TARGET} STATIC ${SOURCES} ${HEADERS})

# propagate includes to other targets
target_link_libraries(${TARGET} fitsio)
target_include_directories(${TARGET} PUBLIC ${CMAKE_SOURCE_DIR}/external)

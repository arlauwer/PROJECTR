set(TARGET ${PROJECT_NAME})

add_executable(${TARGET} main.cpp)

target_link_root(${TARGET} src core)

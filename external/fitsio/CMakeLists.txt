set(TARGET fitsio)

file(GLOB SOURCES "*.c")
file(GLOB HEADERS "*.h")

add_library(${TARGET} STATIC ${SOURCES} ${HEADERS})

# avoid non-ANSI include
add_definitions(-DFF_NO_UNISTD_H=1)

# suppress all compiler warnings
target_compile_options(${TARGET} PRIVATE -w)

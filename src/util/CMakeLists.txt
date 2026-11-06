set(TARGET util)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")
add_library(${TARGET} ${SOURCES})

target_link_root(${TARGET} src)
target_link_root(${TARGET} external pcg)                  # Random.hpp exposes pcg
target_link_root(${TARGET} external SCOPE PRIVATE fitsio) # only used in FitsImage.cpp

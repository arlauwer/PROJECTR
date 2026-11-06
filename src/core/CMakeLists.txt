set(TARGET core)

find_package(TBB REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")
add_library(${TARGET} ${SOURCES})

# PUBLIC: TBB is needed by Batch::for_each, a template in a header
target_link_root(${TARGET} src util vulkan TBB::tbb)

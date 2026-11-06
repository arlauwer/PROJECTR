set(TARGET vulkan)

find_package(Vulkan REQUIRED)

file(GLOB SOURCES CONFIGURE_DEPENDS "*.cpp")
add_library(${TARGET} ${SOURCES})

# PUBLIC: VulkanContext.hpp includes Vulkan headers
target_link_root(${TARGET} src util Vulkan::Vulkan)

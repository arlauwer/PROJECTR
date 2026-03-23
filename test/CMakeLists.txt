set(TARGET test)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

enable_testing()

file(GLOB_RECURSE SOURCES "*.cpp")

add_executable(${TARGET} ${SOURCES})

target_link_libraries(${TARGET} GTest::gtest_main util vulkan core)
target_include_directories(${TARGET} PRIVATE ../src/util ../src/vulkan ../src/core)

include(GoogleTest)
gtest_discover_tests(${TARGET})
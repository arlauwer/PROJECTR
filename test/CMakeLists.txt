set(TARGET tests) # "test" is reserved by CTest

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")
add_executable(${TARGET} ${SOURCES})

target_link_root(${TARGET} src core GTest::gtest)

target_compile_definitions(${TARGET} PRIVATE
    TEST_DIR="${CMAKE_CURRENT_SOURCE_DIR}/"
)

include(GoogleTest)
gtest_discover_tests(${TARGET})

add_library(color_lib INTERFACE)

target_include_directories(color_lib INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")

# testing
option(BUILD_TESTS "Build the tests for the color lib" ON)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif(BUILD_TESTS)

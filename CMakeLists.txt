add_library(color_lib INTERFACE)

target_include_directories(color_lib INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")

# testing
enable_testing()
add_subdirectory(tests)


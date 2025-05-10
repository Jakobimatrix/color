if (NOT FUZZER_ENABLED)

  # only enable other executables, if Fuzzer is not linked. Fuzzer brings its own main!
  Include(FetchContent)

  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.4.0 # or a later release
  )

  FetchContent_MakeAvailable(Catch2)

  enable_testing()

  add_executable(test_color src/test_color.cpp)

  target_link_libraries(test_color 
    PRIVATE
    Catch2::Catch2WithMain
    color_lib_0.0.1
    ${ENVIRONMENT_SETTINGS}
    )

  include(CTest)
  include(Catch)

  catch_discover_tests(test_color)

endif()
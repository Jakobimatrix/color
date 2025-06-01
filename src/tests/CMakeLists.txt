if (NOT FUZZER_ENABLED)

  add_executable(test_color src/test_color.cpp)

  target_link_libraries(test_color 
    PRIVATE
    Catch2::Catch2WithMain
    color_lib_1.0.0
    ${ENVIRONMENT_SETTINGS}
    )

  catch_discover_tests(test_color)

endif()

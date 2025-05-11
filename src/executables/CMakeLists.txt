

if (FUZZER_ENABLED)

  add_executable(fuzzer_color src/fuzz_color_class.cpp)

  target_link_libraries(fuzzer_color 
    PRIVATE 
    color_lib_0.0.1
    ${ENVIRONMENT_SETTINGS}
  )

else()
  # only enable other executables (like tests), if Fuzzer is not linked. Fuzzer brings its own main!
    
  add_executable(color_example src/color_example.cpp)

  # Optionally, you can also install the executable
  install(TARGETS color_example DESTINATION bin)

  target_link_libraries(color_example 
    PRIVATE
    color_lib_0.0.1
    ${ENVIRONMENT_SETTINGS}
  ) 


endif()


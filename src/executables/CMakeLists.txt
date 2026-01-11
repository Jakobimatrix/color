

if (FUZZER_ENABLED)

  generate_fuzzer_name("fuzzer_color" FUZZER_EXE_NAME)

  add_executable(${FUZZER_EXE_NAME} src/fuzz_color_class.cpp)

  target_link_libraries(${FUZZER_EXE_NAME} 
    PRIVATE 
    color_lib_1.0.0
    BuildSettings_FUZZER
  )
endif()

add_executable(color_example src/color_example.cpp)

# Optionally, you can also install the executable
install(TARGETS color_example DESTINATION bin)

target_link_libraries(color_example 
  PRIVATE
  color_lib_1.0.0
  BuildSettings_EXE
) 



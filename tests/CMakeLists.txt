project(test_color_lib)

set(BOOST_INCLUDE_DIRS $boost_installation_prefix/include)


include_directories(${Boost_INCLUDE_DIRS})
link_directories (${Boost_LIBRARY_DIRS})

find_package(Boost COMPONENTS system unit_test_framework filesystem)

if( Boost_FOUND )
  add_executable(test_color test_color.cpp)

  target_link_libraries(test_color
		                  color_lib
                      ${Boost_FILESYSTEM_LIBRARY}
                      ${Boost_SYSTEM_LIBRARY}
                      ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                      )

  # declares a test with our executable
  add_test(NAME color_test COMMAND test_color)
endif()

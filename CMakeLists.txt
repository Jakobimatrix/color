cmake_minimum_required(VERSION 3.16)

# Define project if not already defined (useful for subproject inclusion)
if (NOT DEFINED PROJECT_NAME)
    project(color LANGUAGES CXX)

    # This variable must exist!
    set(ENVIRONMENT_SETTINGS "ProjectSettings")

    add_library("${ENVIRONMENT_SETTINGS}" INTERFACE)

    # ===== Compiler Settings =====
    include(initRepo/cmake/CompilerSetup.cmake)

    # ===== Warnings and Tools =====
    include(initRepo/cmake/CompilerWarnings.cmake)
    set_project_warnings(${ENVIRONMENT_SETTINGS})

    # ===== External Dependencies =====
    include(initRepo/cmake/Includes.cmake)

    # ===== Options =====
    include(initRepo/cmake/Options.cmake)
    #enable_multithreading(${ENVIRONMENT_SETTINGS})
    enable_shared_libraries(${ENVIRONMENT_SETTINGS})

    # ===== Fuzzing =====
    include(initRepo/cmake/ClangFuzzyTests.cmake)

    # ===== CTest with Catch2 =====
    enable_testing()
    setup_catch2_and_ctest()
endif()


add_subdirectory(src/color)
add_subdirectory(src/executables)
add_subdirectory(src/tests)

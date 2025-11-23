# color 

[![C/C++ CI](https://github.com/Jakobimatrix/color/actions/workflows/ubuntu_build_test.yml/badge.svg?branch=main)](https://github.com/Jakobimatrix/color/actions/workflows/ubuntu_build_test.yml)

 - OS: Ubuntu 24.04
 - compiler: clang 19, gcc 13
 - debug + release
 - 32Bit/64Bit
 - tests

 [![Windows C/C++ CI](https://github.com/Jakobimatrix/color/actions/workflows/windows_build_test.yml/badge.svg)](https://github.com/Jakobimatrix/color/actions/workflows/windows_build_test.yml)

 - OS: windows-latest
 - compiler: clang-latest, gcc-latest, msvc-latest
 - debug + release
 - 32Bit/64Bit
 - tests
 ---

This header only class lets you conveniently convert between RGB and HSV and between float [0 - 1] and int [0 - 255].
This class was my playground for teaching myself about templates thus the result might not be the most straightforward implementation for such a "simple feature".


# Build
 - Either use the provided CMakeLists.txt and #include <color/color.hpp>
 - or just #include "color/src/color/include/color.hpp" (relative path)
 - you can build the example with `./build -r -t` (builds in release mode gcc and builds the tests.)
 - For examples see [color_example.cpp](src/executables/src/color_example.cpp) or [test_color.cpp](src/tests/src/test_color.cpp)


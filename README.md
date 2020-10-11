# color

This header only class lets you conveniently convert between RGB and HSV and between float [0 - 1] and int [0 - 255].
This class was my playground for teaching myself about templates thus the result might not be the most straightforward implementation for such a "simple feature".

I also wanted to try out Boost.Test in combination with CMake, so that exists too... 

# Build
 - Either use the provided CMakeLists.txt and #include <color.hpp>
 - or just #include "<include/color.hpp>" (relative path)
 - you can build the example with `g++ example.cpp -o example_executable`
   - Will fail the first time (see comment in example.cpp and fix yourself)
   - execute with `./example_executable`
 - For the tests go into build `cd build`
 - run script `./buildAndRunTests.sh`
 - (an installation of Boost (works with 1.65.1) is required for the tests)

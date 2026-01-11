/**
 * @file fuzz_color_class.cpp
 * @brief contains an nearly minimal example how to fuzz a function.
 *
 * @detail The Fuzzer creates a more or less (less it does some clever things) random binary string and tries to kill your application.
 *         1) Build in debug mode with clang
 *         2) Run ./fuzz_color_class -print_final_stats=1
 *         3) After it ended a crash-<md5hash> binary file was created containing the input that crashed the application
 *         4) run ./fuzz_color_class crash-<md5hash> and attach the debugger.
 *
 **/

#include <color/color.hpp>

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>

namespace {
template <typename T>
  requires std::is_trivially_copyable_v<T>
constexpr T read(const unsigned char* data, size_t* offset) {
  T value{};
  std::memcpy(&value, data + *offset, sizeof(T));  // NOLINT (cppcoreguidelines-pro-bounds-pointer-arithmetic) I get raw data... SO I have to deal with that.
  *offset += sizeof(T);
  return value;
}

/**
 * @brief This bad function initializes different color classes with the given bits and checks if any given bits could crash the methods of the class.
 *
 * @param data Pointer to data begin.
 * @param size Size of the Data.
 * @return true if we get the right size for the data.
 */
constexpr bool badFunction(const unsigned char* data, size_t size) {
  constexpr size_t NUM_BYTES_4_4_INTS   = 16;
  constexpr size_t NUM_BYTES_4_4_FLOATS = 16;
  if (size != NUM_BYTES_4_4_INTS + NUM_BYTES_4_4_FLOATS) {  // Ensure enough data for both integer and float interpretations
    return false;  // Not enough data to proceed
  }
  size_t offset = 0;
  {
    // Interpret the first bytes as integers for pigments
    const int r = read<int32_t>(data, &offset);
    const int g = read<int32_t>(data, &offset);
    const int b = read<int32_t>(data, &offset);
    const int a = read<int32_t>(data, &offset);

    // Construct RGB and HSV objects with integer values
    const color::RGB<int> rgb(r, g, b);
    const color::RGB<int, 4> rgba(r, g, b, a);
    const color::HSV<int> hsv(r, g, b);  // Interpreting r, g, b as h, s, v
    const color::HSV<int, 4> hsva(r, g, b, a);

    // Convert between RGB and HSV
    const color::HSV<double> hsv_from_rgb(color::convertToHSV(color::RGB<double>(rgb)));
    const color::RGB<double> rgb_from_hsv(color::convertToRGB(color::HSV<double>(hsv)));

    // Output to ensure no exceptions are thrown
    std::cout << "Integer-based colors:\n";
    std::cout << rgb << "\n";
    std::cout << rgba << "\n";
    std::cout << hsv << "\n";
    std::cout << hsva << "\n";
  }


  {
    // Interpret the next bytes as floats for pigments
    const float r = read<float>(data, &offset);
    const float g = read<float>(data, &offset);
    const float b = read<float>(data, &offset);
    const float a = read<float>(data, &offset);

    // Construct RGB and HSV objects with float values
    const color::RGB<float> rgb(r, g, b);
    const color::RGB<float, 4> rgba(r, g, b, a);
    const color::HSV<float> hsv(r, g, b);  // Interpreting r, g, b as h, s, v
    const color::HSV<float, 4> hsva(r, g, b, a);

    // Convert between RGB and HSV
    const color::HSV<double> hsv_from_rgb(color::convertToHSV(color::RGB<double>(rgb)));
    const color::RGB<double> rgb_from_hsv(color::convertToRGB(color::HSV<double>(hsv)));

    // Output to ensure no exceptions are thrown
    std::cout << "Float-based colors:\n";
    std::cout << rgb << "\n";
    std::cout << rgba << "\n";
    std::cout << hsv << "\n";
    std::cout << hsva << "\n";
  }

  return true;
}

}  // end anonymous namespace


extern "C" int LLVMFuzzerTestOneInput(const unsigned char* data, unsigned long size) {

  return static_cast<int>(badFunction(data, static_cast<size_t>(size)));
}

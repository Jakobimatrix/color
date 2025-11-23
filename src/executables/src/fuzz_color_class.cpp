/**
 * @file fuzz_color_class.cpp
 * @brief contains an nearly minimal example how to fuzz a function.
 *
 * @detail The Fuzzer creates a more or less (less it does some clever things) random binary string and tries to kill your application.
 *         1) Build in release mode with clang
 *         2) Run ./fuzz_color_class -print_final_stats=1
 *         3) After it ended a crash-<md5hash> binary file was created containing the input that crashed the application
 *         4) Build in debug mode
 *         5) Run ./fuzz_color_class crash-<md5hash> and attach the debugger, than hit enter
 *
 * @date 30.03.2025
 * @author Jakob Wandel
 * @version 1.0
 **/

#include <color/color.hpp>


#include <concepts>
#include <cstring>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdint>
#include <stdexcept>
#include <type_traits>
#include <vector>

// Concept must be outside the anonymous namespace for C++20

template <typename ByteType>
concept ByteTypeAllowed =
  !std::is_const_v<ByteType> &&
  (std::same_as<ByteType, char> || std::same_as<ByteType, unsigned char> ||
   std::same_as<ByteType, signed char> || std::same_as<ByteType, std::uint8_t>);

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

/**
 * @brief Reads a binary file into a vector of bytes.
 *
 * @tparam ByteType Must be either char, unsigned char, or std::uint8_t.
 * @param path Path to the file.
 * @return std::vector<uint8_t> Contents of the file.
 * @throws std::runtime_error if the file can't be opened.
 */

template <ByteTypeAllowed ByteType>
std::vector<ByteType> readFileBinary(const std::filesystem::path& path) {
  std::ifstream file(path, std::ios::binary | std::ios::ate);
  if (!file) {
    throw std::runtime_error("Failed to open file: " + path.string());
  }

  const std::streampos size = file.tellg();

  if (size < 0) {
    throw std::runtime_error("Failed to determine file size: " + path.string());
  }
  if (static_cast<uintmax_t>(size) > std::numeric_limits<size_t>::max()) {
    throw std::runtime_error("File too large to fit in memory: " + path.string());
  }

  file.seekg(0, std::ios::beg);
  std::vector<ByteType> buffer(static_cast<size_t>(size));
  if constexpr (std::is_same<char, ByteType>()) {
    if (!file.read(buffer.data(), size)) {
      throw std::runtime_error("Failed to read file: " + path.string());
    }
  } else {
    if (!file.read(reinterpret_cast<char*>(buffer.data()),  // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
                   static_cast<std::streamsize>(size))) {
      // reinterpret_cast is required: std::ifstream::read takes char*,
      // and buffer.data() may be uint8_t* / unsigned char*, etc. This cast is safe and idiomatic.
      throw std::runtime_error("Failed to read file: " + path.string());
    }
  }

  return buffer;
}

}  // end anonymous namespace


#if FUZZER_ACTIVE
// we compiled in release mode, The fuzzer can do its magic


extern "C" int LLVMFuzzerTestOneInput(const unsigned char* data, unsigned long size) {

  return static_cast<int>(badFunction(data, static_cast<size_t>(size)));
}


#else
// We compiled in debug mode, you can call badFunction yourself with the data which crashed badFunction and see what went wrong

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0]  // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic) Thats how its done unfortunately
              << " <file_path>\n";
    return 1;
  }

  const std::filesystem::path file_path(argv[1]);  // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic) Thats how its done unfortunately

  if (!std::filesystem::exists(file_path)) {
    std::cerr << "File does not exist: " << file_path << "\n";
    return 1;
  }

  try {
    auto data = readFileBinary<unsigned char>(file_path);
    std::cout
      << "\nFile found and read. Now attach debugger and press enter.\n";
    std::cout
      << "If you get an error from ptrace 'Could not attach to the process.' "
         "Use 'echo 0 | sudo tee /proc/sys/kernel/yama/ptrace_scope' to relax "
         "restrictions temporarily.\n"
      << std::flush;
    getchar();
    return static_cast<int>(badFunction(data.data(), data.size()));
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  return 0;
}


#endif

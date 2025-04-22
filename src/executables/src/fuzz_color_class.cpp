/**
* @file fuzz_color_class.hpp
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

#include <concepts>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

#include <color/color.hpp>

/**
 * @brief This bad function is delibritly wrong.
 * If the given data is of size 3 and equals "FUZ" we have an access violation.
 *
 * @param data Pointer to data begin.
 * @param size Size of the Data.
 * @return true if the input starts with "FUZZ".
 */
inline bool badFunction(const unsigned char *data, size_t size) {
  if (size < 16) { // Ensure enough data for both integer and float interpretations
    return false; // Not enough data to proceed
  }

  try {
    // Interpret the first 4 bytes as integers for pigments
    int r = static_cast<int>(data[0]) |
            (static_cast<int>(data[1]) << 8) |
            (static_cast<int>(data[2]) << 16) |
            (static_cast<int>(data[3]) << 24);
    int g = static_cast<int>(data[4]) |
            (static_cast<int>(data[5]) << 8) |
            (static_cast<int>(data[6]) << 16) |
            (static_cast<int>(data[7]) << 24);
    int b = static_cast<int>(data[8]) |
            (static_cast<int>(data[9]) << 8) |
            (static_cast<int>(data[10]) << 16) |
            (static_cast<int>(data[11]) << 24);
    int a = static_cast<int>(data[12]) |
            (static_cast<int>(data[13]) << 8) |
            (static_cast<int>(data[14]) << 16) |
            (static_cast<int>(data[15]) << 24);

    // Construct RGB and HSV objects with integer values
    color::RGB<int> rgb(r, g, b);
    color::RGB<int, 4> rgba(r, g, b, a);
    color::HSV<int> hsv(r, g, b); // Interpreting r, g, b as h, s, v
    color::HSV<int, 4> hsva(r, g, b, a);

    // Convert between RGB and HSV
    color::HSV<double> hsv_from_rgb(rgb);
    color::RGB<double> rgb_from_hsv(hsv);

    // Output to ensure no exceptions are thrown
    std::cout << "Integer-based colors:" << std::endl;
    std::cout << rgb << std::endl;
    std::cout << rgba << std::endl;
    std::cout << hsv << std::endl;
    std::cout << hsva << std::endl;

  } catch (const std::exception &e) {
    std::cerr << "Exception caught during integer interpretation: " << e.what() << std::endl;
    return false;
  }

  if (size < 32) { // Ensure enough data for float interpretation
    return true; // Skip float interpretation if not enough data
  }

  try {
    // Interpret the next 16 bytes as floats for pigments
    float r = *reinterpret_cast<const float *>(&data[16]);
    float g = *reinterpret_cast<const float *>(&data[20]);
    float b = *reinterpret_cast<const float *>(&data[24]);
    float a = *reinterpret_cast<const float *>(&data[28]);

    // Construct RGB and HSV objects with float values
    color::RGB<float> rgb(r, g, b);
    color::RGB<float, 4> rgba(r, g, b, a);
    color::HSV<float> hsv(r, g, b); // Interpreting r, g, b as h, s, v
    color::HSV<float, 4> hsva(r, g, b, a);

    // Convert between RGB and HSV
    color::HSV<double> hsv_from_rgb(rgb);
    color::RGB<double> rgb_from_hsv(hsv);

    // Output to ensure no exceptions are thrown
    std::cout << "Float-based colors:" << std::endl;
    std::cout << rgb << std::endl;
    std::cout << rgba << std::endl;
    std::cout << hsv << std::endl;
    std::cout << hsva << std::endl;

  } catch (const std::exception &e) {
    std::cerr << "Exception caught during float interpretation: " << e.what() << std::endl;
    return false;
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

 template <typename ByteType>
 concept ByteTypeAllowed =
     !std::is_const_v<ByteType> &&
     (std::same_as<ByteType, char> ||
      std::same_as<ByteType, unsigned char> ||
      std::same_as<ByteType, signed char> ||
      std::same_as<ByteType, std::uint8_t>);
template <ByteTypeAllowed ByteType>
std::vector<ByteType> readFileBinary(const std::filesystem::path& path) {
  std::ifstream file(path, std::ios::binary | std::ios::ate);
  if (!file) {
    throw std::runtime_error("Failed to open file: " + path.string());
  }

  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);

  std::vector<ByteType> buffer(static_cast<size_t>(size));
  if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
    throw std::runtime_error("Failed to read file: " + path.string());
  }

  return buffer;
}


#if FUZZER_ACTIVE
// we compiled in release mode, The fuzzer can do its magic


extern "C" int LLVMFuzzerTestOneInput(const unsigned char *data, unsigned long size) {

  return static_cast<int>(badFunction(data, static_cast<size_t>(size)));
}


#else
// We compiled in debug mode, you can call badFunction yourself with the data which crashed badFunction and see what went wrong

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <file_path>\n";
    return 1;
  }

  std::filesystem::path file_path(argv[1]);

  if (!std::filesystem::exists(file_path)) {
    std::cerr << "File does not exist: " << file_path << "\n";
    return 1;
  }

  try {
    auto data = readFileBinary<unsigned char>(file_path);
    printf("\nFile found and read. Now attach debugger and press enter.\n");
    getchar();
    return static_cast<int>(badFunction(data.data(), data.size()));
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  return 0;
}


#endif

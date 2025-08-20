#include <array>
#include <color/color.hpp>

#include <cstdio>
#include <iostream>

// NOLINTBEGIN(readability-magic-numbers) // yes this is an example, it contains example numbers. There is no benefit in naming them exampleNumber123

int main() {
  std::array<int, 3> const pigments_i    = {{0, 100, 255}};
  std::array<double, 3> const pigments_d = {{0., 0.3921568627, 1.}};

  // initiation with array or 3 single numbers in order h,s,v
  color::HSV<int> const hsvi(pigments_i);
  color::HSV<int> const hsvi2(0, 100, 255);
  color::HSV<int> const hsvi3(pigments_d);
  color::HSV<int> const hsvi4(0., 0.3921568627, 1.);

  // initiation with other class of HSV from type integral or floating point
  color::HSV<double> hsvd(hsvi);

  std::cout << "---hsv2rgb---" << '\n';

  // conversion to RGB
  std::cout << hsvi << "to:" << '\n';
  color::RGB<double> rgbid{color::convertToRGB(hsvd)};
  std::cout << color::RGB<int>(rgbid) << '\n';


  std::cout << "---rgb2hsv---" << '\n';

  // conversion to HSV
  std::cout << rgbid << "to:" << '\n';
  std::cout << color::convertToHSV(rgbid) << '\n';


  std::cout << "---access---" << '\n';

  // access color data like this:
  std::cout << " H:" << hsvd.h() << " S:" << hsvd.s() << " V:" << hsvd.v() << '\n';
  std::cout << " R:" << rgbid.r() << " G:" << rgbid.g() << " B:" << rgbid.b() << '\n';
  // or this:
  constexpr size_t NUM_PIGMENTS = 3;
  for (size_t pigmentNr = 0; pigmentNr < NUM_PIGMENTS; ++pigmentNr) {
    std::cout << " Data<" << pigmentNr << ">:" << rgbid[pigmentNr] << '\n';
  }

  std::cout << "---alpha value---" << '\n';

  // To add a alpha value to the color, you need
  // to add 4 as the second template parameter
  color::RGB<int, 4> rgba(22, 33, 44);
  std::cout << "The default value is 255 (or 1. depending on T)" << '\n';
  std::cout << rgba << '\n';
  std::cout << "You can define alpha at construction too: " << '\n';
  rgba = color::RGB<int, 4>(22, 33, 44, 55);
  std::cout << rgba << '\n';

  std::cout << "Oh and you can access a also like this: "
            << "rgba.a = " << rgba.a() << '\n'
            << "or like this: rgba[3] = " << rgba[3] << '\n';

  color::RGB<int> rgb(22, 33, 44);

  // This will fail at compile time:
  // std::cout << rgb.a() << std::endl;

  // but this wont! If you are lucky you get a SEGFAULT:
  // std::cout << rgb[3] << std::endl; // actually modern clang catches this with [maybe-uninitialized]


  // you louse information if you do this:
  rgb = color::RGB<int>(rgba);

  std::getchar();
  return 0;
}

// NOLINTEND(readability-magic-numbers)

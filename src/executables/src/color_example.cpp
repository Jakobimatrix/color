#include <color/color.hpp>

#include <iostream>

int main() {
  std::array<int, 3> pigments_i = {{0, 100, 255}};
  std::array<double, 3> pigments_d = {{0., 0.3921568627, 1.}};

  // initiation with array or 3 single numbers in order h,s,v
  color::HSV<int> hsvi(pigments_i);
  color::HSV<int> hsvi2(0, 100, 255);
  color::HSV<int> hsvi3(pigments_d);
  color::HSV<int> hsvi4(0., 0.3921568627, 1.);

  // initiation with other class of HSV from type integral or floating point
  color::HSV<double> hsvd(hsvi);

  std::cout << "---hsv2rgb---" << std::endl;

  // conversion to RGB
  std::cout << hsvi << "to:" << std::endl;
  color::RGB<double> rgbid{color::convertToRGB(hsvd)};
  std::cout << color::RGB<int>(rgbid) << std::endl;

  
  std::cout << "---rgb2hsv---" << std::endl;

  // conversion to HSV
  std::cout << rgbid << "to:" << std::endl;
  std::cout << color::convertToHSV(rgbid) << std::endl;


  std::cout << "---access---" << std::endl;

  // access color data like this:
  std::cout << " H:" << hsvd.h() << " S:" << hsvd.s() << " V:" << hsvd.v() << std::endl;
  std::cout << " R:" << rgbid.r() << " G:" << rgbid.g() << " B:" << rgbid.b() << std::endl;
  // or this:
  for (unsigned int i = 0; i < 3; i++) {
    std::cout << " Data<" << i << ">:" << rgbid[i] << std::endl;
  }

  std::cout << "---alpha value---" << std::endl;

  // To add a alpha value to the color, you need
  // to add 4 as the second template parameter
  color::RGB<int, 4> rgba(22, 33, 44);
  std::cout << "The default value is 255 (or 1. depending on T)" << std::endl;
  std::cout << rgba << std::endl;
  std::cout << "You can define alpha at construction too: " << std::endl;
  rgba = color::RGB<int, 4>(22, 33, 44, 55);
  std::cout << rgba << std::endl;

  std::cout << "Oh and you can access a also like this: "
            << "rgba.a = " << rgba.a() << std::endl
            << "or like this: rgba[3] = " << rgba[3] << std::endl;

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

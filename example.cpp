#include <iostream>

#include "include/color.hpp"

int main() {
  std::array<int, 3> pigments_i = {{0, 100, 255}};
  std::array<double, 3> pigments_d = {{0., 0.3921568627, 1.}};

  // initiation with array or 3 single numbers//in order h,s,v
  color::HSV<int> hsvi(pigments_i);
  color::HSV<int> hsvi2(0, 100, 255);
  color::HSV<int> hsvi3(pigments_d);
  color::HSV<int> hsvi4(0., 0.3921568627, 1.);

  // initiation with other class of HSV from type integral or floating point
  color::HSV<double> hsvd(hsvi);

  // initiate RGB with class HSV. Array or 3 single numbers in order r,g,b
  color::RGB<int> rgbii(hsvi);
  color::RGB<int> rgbid(hsvd);
  color::RGB<double> rgbdi(hsvi);
  color::RGB<double> rgbdd(hsvd);

  // assigment between different types and color spaces is possible too.
  color::RGB<int> rgbii2 = rgbdi;
  color::RGB<int> rgbii3 = hsvi;
  color::RGB<int> rgbii4 = hsvd;

  color::RGB<double> rgbdi2 = rgbdi;
  color::RGB<double> rgbdi3 = hsvi;
  color::RGB<double> rgbdi4 = hsvd;

  // output
  std::cout << rgbdi2;

  std::cout << "---hsv2rgb---" << std::endl;

  // conversion to RGB
  std::cout << hsvi << "to:" << std::endl;
  std::cout << color::convertToRGB(hsvi);
  auto x = color::convertToRGB(hsvi);
  std::cout << color::convertToRGB(hsvd);

  std::cout << "---rgb2hsv---" << std::endl;

  // conversion to HSV
  std::cout << x << "to:" << std::endl;
  std::cout << color::convertToHSV(x);
  std::cout << color::HSV<int>(color::convertToHSV(x));
  std::cout << color::convertToHSV(rgbid);

  std::cout << "---access---" << std::endl;

  // access color data like this:
  std::cout << " H:" << hsvd.h << " S:" << hsvd.s << " V:" << hsvd.v << std::endl;
  std::cout << " R:" << rgbid.r << " G:" << rgbid.g << " B:" << rgbid.b << std::endl;
  // or this:
  for (unsigned int i = 0; i < 3; i++) {
    std::cout << " Data<" << i << ">:" << rgbii2[i] << std::endl;
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
            << "rgba.a = " << rgba.a << std::endl
            << "or like this: rgba[3] = " << rgba[3] << std::endl;

  color::RGB<int> rgb(22, 33, 44);

  // This will fail at compile time:
  std::cout << rgb.a << std::endl;

  // but this wont! If you are lucky you get a SEGFAULT:
  std::cout << rgb[3] << std::endl;


  // you louse information if you do this:
  rgb = color::RGB<int>(rgba);

  std::getchar();
  return 0;
}

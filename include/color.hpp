/*
 + Usage:
  std::array<int, 3> pigments_i = {{0, 100, 255}};
  std::array<double, 3> pigments_d = {{0., 0.3921568627, 1.}};

  // initiation with array or 3 single numbers//in order h,s,i
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
  std::cout << " H:" << hsvd.h << " S:" << hsvd.s << " V:" << hsvd.v
            << std::endl;
  std::cout << " R:" << rgbid.r << " G:" << rgbid.g << " B:" << rgbid.b
            << std::endl;
  // or this
  for (unsigned int i = 0; i < 3; i++) {
    std::cout << " Data<" << i << ">:" << rgbii2[i] << std::endl;
  }
*/

#ifndef COLOR_H
#define COLOR_H

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <type_traits>

namespace func {
// https://stackoverflow.com/questions/26434128/how-to-make-is-arithmeticmyclassvalue-to-be-true
template <class...>
struct make_void {
  using type = void;
};
template <class... Ts>
using void_t = typename make_void<Ts...>::type;

template <class T, class = void>
struct supports_arithmetic_operations : std::false_type {};

template <class T>
struct supports_arithmetic_operations<T,
                                      void_t<decltype(std::declval<T>() + std::declval<T>()),
                                             decltype(std::declval<T>() - std::declval<T>()),
                                             decltype(std::declval<T>() * std::declval<T>()),
                                             decltype(std::declval<T>() / std::declval<T>())>>
    : std::true_type {};
}  // namespace func

namespace color {


template <class T, size_t NUM_VALUES, typename Alpha = void>
class BaseColor;

template <class T, size_t NUM_VALUES>
class BaseColor<T, NUM_VALUES, std::enable_if_t<(NUM_VALUES == 4)>> {
 public:
  std::array<T, NUM_VALUES> pigment;  // a fraction between 0 and 1 OR int [0 - 255]
  T &a = pigment[3];
  static constexpr bool has_alpha = true;

  BaseColor() {
    if (std::is_floating_point<T>::value) {
      a = 1;
    } else {
      a = 255;
    }
  }
};

template <class T, size_t NUM_VALUES>
class BaseColor<T, NUM_VALUES, std::enable_if_t<(NUM_VALUES != 4)>> {
 public:
  std::array<T, NUM_VALUES> pigment;  // a fraction between 0 and 1 OR int [0 - 255]
  static constexpr bool has_alpha = false;
};


template <class T, size_t NUM_VALUES = 3>
class Color : public BaseColor<T, NUM_VALUES> {

 protected:
  Color() {
    static_assert(NUM_VALUES == 3 || NUM_VALUES == 4,
                  "The number of values (second template parameter) can only "
                  "be 3 (without alpha) or 4 (with alpha)");
  }

 public:
  template <class T_, size_t NUM_VALUES_>
  Color(const std::array<T_, NUM_VALUES_> &pigment_) {
    static_assert(func::supports_arithmetic_operations<T_>::value,
                  "You cannot initiate a color with given type ");

    // we either cut a if NUM_VALUES == 3 and NUM_VALUES_ == 4,
    // or we assume a = 100% if
    // NUM_VALUES_ == 3 but NUM_VALUES == 4;
    size_t min = std::min(NUM_VALUES_, NUM_VALUES);

    if (std::is_same<T_, T>::value) {
      // pigment[i] = pigment_[i]; // this doesnt work???
      for (unsigned int i = 0; i < min; i++) {
        this->pigment[i] = pigment_[i];
      }
      // pigment = pigment_;
    } else if (isFloatingpoint() && std::is_integral<T_>::value) {
      for (unsigned int i = 0; i < min; i++) {
        this->pigment[i] = static_cast<T>(pigment_[i]) / 255.0;
      }
      // pigment = static_cast<T>(pigment_) / 255.0;
    } else if (isIntegral() && std::is_floating_point<T_>::value) {
      for (unsigned int i = 0; i < min; i++) {
        this->pigment[i] = std::round(pigment_[i] * 255.0);
      }
    }
  }

  bool isIntegral() const { return std::is_integral<T>::value; }

  bool isFloatingpoint() const { return std::is_floating_point<T>::value; }

 public:
  virtual std::string pigmentName(unsigned int) const = 0;
  virtual std::string getColorTypeName() const = 0;

  friend std::ostream &operator<<(std::ostream &os, const Color &c) {
    os << c.getColorTypeName() << std::endl;
    for (unsigned int i = 0; i < NUM_VALUES; i++) {
      os << "[" << c.pigmentName(i) << ": " << c.pigment[i] << "]";
    }
    os << std::endl;
    return os;
  }

  T &operator[](unsigned int x) { return this->pigment[x]; }
};

template <class T, size_t NUM_VALUES = 3>
class RGB;
template <class T, size_t NUM_VALUES = 3>
class HSV;

static RGB<double> convertToRGB(const HSV<double> &hsv);
static HSV<double> convertToHSV(const RGB<double> &rgb);

template <class T, size_t NUM_VALUES>
class RGB : public Color<T, NUM_VALUES> {
 public:
  // https://isocpp.org/wiki/faq/templates#nondependent-name-lookup-members
  // We need to tell the compiler explicitly that the names are dependent on the
  // instantiation of the parent. This is because the template parent of a
  // template class is not instantiated during the compilation pass that first
  // examines the template.
  T &r = this->pigment[0];
  T &g = this->pigment[1];
  T &b = this->pigment[2];

  RGB() {}

  template <class T_, size_t NUM_VALUES_>
  RGB(const RGB<T_, NUM_VALUES_> &rgb) : Color<T, NUM_VALUES>(rgb.pigment) {}

  template <class T_, size_t NUM_VALUES_>
  RGB(const HSV<T_, NUM_VALUES_> &hsv) {
    const RGB<T, NUM_VALUES> temp = convertToRGB(hsv);
    *this = temp;
  }

  template <class T_, size_t NUM_VALUES_>
  RGB(const std::array<T_, NUM_VALUES_> &pigments)
      : Color<T, NUM_VALUES>(pigments) {}

  template <class T_>
  RGB(T_ r, T_ g, T_ b) : Color<T, NUM_VALUES>(std::array<T_, 3>{{r, g, b}}){};

  template <class T_>
  RGB(T_ r, T_ g, T_ b, T_ a)
      : Color<T, NUM_VALUES>(std::array<T_, 4>{{r, g, b, a}}) {}

  RGB<T, NUM_VALUES> &operator=(const RGB<T, NUM_VALUES> &rgb) {
    if (&rgb == this) {
      return *this;
    }
    // constructor deals with different T
    const RGB<T, NUM_VALUES> temp(rgb);
    std::copy(&temp.pigment[0], &temp.pigment[0] + NUM_VALUES, &this->pigment[0]);
    return *this;
  }

  template <class T_, size_t NUM_VALUES_>
  RGB<T, NUM_VALUES> &operator=(const HSV<T_, NUM_VALUES_> &hsv) {
    const RGB<T, NUM_VALUES> temp = convertToRGB(hsv);
    *this = temp;
    return *this;
  }

  std::string pigmentName(unsigned int i) const override {
    switch (i) {
      case 0:
        return "R";
      case 1:
        return "G";
      case 2:
        return "B";
      case 3:
        if (!this->has_alpha) {
          assert(("This color has no alpha value!", i == 0 || i == 1 || i == 2));
        }
        return "A";
      default:
        assert(("pigmentName only supports i in [0, 1, 2]", i == 0 || i == 1 || i == 2));
        return "?";
    }
  }

  std::string getColorTypeName() const override {
    if (std::is_same<T, int>::value) {
      return "RGB_integer";
    } else {
      return "RGB_double";
    }
  }
};

template <class T, size_t NUM_VALUES>
class HSV : public Color<T, NUM_VALUES> {
 public:
  T &h = this->pigment[0];
  T &s = this->pigment[1];
  T &v = this->pigment[2];

  HSV() {}

  template <class T_, size_t NUM_VALUES_>
  HSV(const HSV<T_, NUM_VALUES_> &hsv) : Color<T, NUM_VALUES>(hsv.pigment) {}

  template <class T_, size_t NUM_VALUES_>
  HSV(const RGB<T_, NUM_VALUES_> &rgb) {
    const HSV<T> temp = convertToHSV(rgb);
    *this = temp;
  }

  template <class T_, size_t NUM_VALUES_>
  HSV(const std::array<T_, NUM_VALUES_> &pigments)
      : Color<T, NUM_VALUES>(pigments) {}

  template <class T_>
  HSV(T_ h, T_ s, T_ v) : Color<T, NUM_VALUES>(std::array<T_, 3>{{h, s, v}}) {}

  template <class T_>
  HSV(T_ h, T_ s, T_ v, T_ a)
      : Color<T, NUM_VALUES>(std::array<T_, 4>{{h, s, v, a}}) {}

  HSV<T, NUM_VALUES> &operator=(const HSV<T, NUM_VALUES> &hsv) {
    if (&hsv == this) {
      return *this;
    }
    // constructor deals with different T
    const HSV<T, NUM_VALUES> temp(hsv);
    std::copy(&temp.pigment[0], &temp.pigment[0] + NUM_VALUES, &this->pigment[0]);
    return *this;
  }

  template <class T_, size_t NUM_VALUES_>
  HSV<T, NUM_VALUES> &operator=(const RGB<T_, NUM_VALUES_> &rgb) {
    const HSV<T, NUM_VALUES> temp = convertToHSV(rgb);
    *this = temp;
    return *this;
  }

  std::string pigmentName(unsigned int i) const override {
    switch (i) {
      case 0:
        return "H";
      case 1:
        return "S";
      case 2:
        return "V";
      case 3:
        if (!this->has_alpha) {
          assert(("This color has no alpha value!", i == 0 || i == 1 || i == 2));
        }
        return "A";
      default:
        assert(("pigmentName only supports i in [0, 1, 2]", i == 0 || i == 1 || i == 2));
        return "?";
    }
  }

  std::string getColorTypeName() const override {
    if (std::is_same<T, int>::value) {
      return "HSV_integer";
    } else {
      return "HSV_double";
    }
  }
};

// r[0-1], g[0-1], b[0-1] -> h[0-1], s[0-1], v[0-1]
static RGB<double> convertToRGB(const HSV<double> &hsv) {

  // https://www.rapidtables.com/convert/color/hsv-to-rgb.html

  const double h_grad = hsv.h * 360.;
  const double C = hsv.v * hsv.s;
  const double X = C * (1. - std::abs(std::fmod(h_grad / 60., 2.) - 1.));
  const double m = (hsv.v - C);

  if (h_grad < 60.) {
    return RGB<double>(C + m, X + m, m);
  } else if (h_grad < 120.) {
    return RGB<double>(X + m, C + m, m);
  } else if (h_grad < 180.) {
    return RGB<double>(m, C + m, X + m);
  } else if (h_grad < 240.) {
    return RGB<double>(m, X + m, C + m);
  } else if (h_grad < 300.) {
    return RGB<double>(X + m, m, C + m);
  } else {
    return RGB<double>(C + m, m, X + m);
  }
}

// h[0-1], s[0-1], v[0-1] -> r[0-1], g[0-1], b[0-1]
static HSV<double> convertToHSV(const RGB<double> &rgb) {
  HSV<double> hsv;
  // https://www.rapidtables.com/convert/color/rgb-to-hsv.html

  const double Cmax = std::max(std::max(rgb.r, rgb.g), rgb.b);
  const double Cmin = std::min(std::min(rgb.r, rgb.g), rgb.b);
  const double delta = Cmax - Cmin;

  constexpr double SMALL_NUMBER = 0.00000001;

  if (delta < SMALL_NUMBER) {
    hsv.h = 0;
  } else if (std::abs(rgb.r - Cmax) < SMALL_NUMBER) {
    hsv.h = 60. * std::fmod((rgb.g - rgb.b) / delta, 6.) / 360.;
  } else if (std::abs(rgb.g - Cmax) < SMALL_NUMBER) {
    hsv.h = 60. * (((rgb.b - rgb.r) / delta) + 2.) / 360.;
  } else {
    hsv.h = 60. * (((rgb.r - rgb.g) / delta) + 4.) / 360.;
  }

  if (hsv.h < 0) {
    hsv.h += 1.;
  }

  if (Cmax < SMALL_NUMBER) {
    hsv.s = 0;
  } else {
    hsv.s = delta / Cmax;
  }

  hsv.v = Cmax;

  return hsv;
}

}  // namespace color

#endif

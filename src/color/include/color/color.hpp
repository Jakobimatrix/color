/**
* @file color.hpp
* @brief contains template classes to define rgb or hsv color which can be converted into each other.
*
* @date 30.03.2025
* @author Jakob Wandel
* @version 1.0
**/

#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <type_traits>
#include <concepts>

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

  constexpr T a() const { return this->pigment[3]; }
  constexpr T &a() { return this->pigment[3]; }

  constexpr BaseColor() {
    if (std::is_floating_point<T>::value) {
      a() = 1;
    } else {
      a() = 255;
    }
  }
};

template <class T, size_t NUM_VALUES>
class BaseColor<T, NUM_VALUES, std::enable_if_t<(NUM_VALUES != 4)>> {
 public:
  std::array<T, NUM_VALUES> pigment;  // a fraction between 0 and 1 OR int [0 - 255]
};


template <class T, size_t NUM_VALUES = 3>
class Color : public BaseColor<T, NUM_VALUES> {

 protected:
  constexpr Color() {
    static_assert(NUM_VALUES == 3 || NUM_VALUES == 4,
                  "The number of values (second template parameter) can only "
                  "be 3 (without alpha) or 4 (with alpha)");
  }

  virtual ~Color() = default;
  constexpr Color(const Color &c) = default;
  constexpr Color(Color &&c) = default;
  constexpr Color &operator=(const Color &c) = default;
  constexpr Color &operator=(Color &&c) = default;

 public:
  template <class T_, size_t NUM_VALUES_>
  constexpr Color(const std::array<T_, NUM_VALUES_> &pigment_) {
    static_assert(func::supports_arithmetic_operations<T_>::value,
                  "You cannot initiate a color with given type ");

    constexpr size_t min = std::min(NUM_VALUES_, NUM_VALUES);

    if constexpr(std::is_same<T_, T>::value) {
      for (size_t i = 0; i < min; ++i) {
        this->pigment[i] = pigment_[i];
      }
    }else if constexpr((std::is_floating_point_v<T> && std::is_floating_point_v<T_>) || 
                      (std::is_integral_v<T> && std::is_integral_v<T_>)) {
      for (size_t i = 0; i < min; ++i) {
        this->pigment[i] = static_cast<T>(pigment_[i]);
      }
    } else if constexpr (std::is_floating_point_v<T> && std::is_integral_v<T_>) {
      for (size_t i = 0; i < min; ++i) {
        this->pigment[i] = static_cast<T>(pigment_[i]) / static_cast<T>(255.0);
      }
    } else if constexpr (std::is_integral_v<T> && std::is_floating_point_v<T_>) {
      for (size_t i = 0; i < min; ++i) {
        this->pigment[i] = static_cast<T>(std::round(pigment_[i] * 255.0));
      }
    }
  }

  constexpr bool isIntegral() const { return std::is_integral_v<T>; }

  constexpr bool isFloatingpoint() const { return std::is_floating_point_v<T>; }

 public:
  virtual std::string pigmentName(size_t) const = 0;
  virtual std::string getColorTypeName() const = 0;

  friend std::ostream &operator<<(std::ostream &os, const Color &c) {
    os << c.getColorTypeName() << std::endl;
    for (size_t i = 0; i < NUM_VALUES; ++i) {
      os << "[" << c.pigmentName(i) << ": " << c.pigment[i] << "]";
    }
    os << std::endl;
    return os;
  }

  constexpr T &operator[](size_t x) { return this->pigment[x]; }
  constexpr T operator[](size_t x) const { return this->pigment[x]; }
};

template <typename T, typename T_, size_t NUM_VALUES, size_t NUM_VALUES_>
concept NeedsConversation = !(std::is_same_v<T_, T> && NUM_VALUES_ == NUM_VALUES);



template <class T, size_t NUM_VALUES=3>
class RGB : public Color<T, NUM_VALUES> {
 public:

  constexpr static bool has_alpha = (NUM_VALUES == 4);

  constexpr RGB() {}
  constexpr RGB(const RGB &rgb) = default;
  constexpr RGB(RGB &&rgb) = default;
  constexpr RGB &operator=(const RGB &rgb) = default;
  constexpr RGB &operator=(RGB &&rgb) = default;
  ~RGB() = default;  

  template <class T_, size_t NUM_VALUES_>
  requires color::NeedsConversation<T, T_, NUM_VALUES, NUM_VALUES_>
  constexpr RGB(const RGB<T_, NUM_VALUES_> &rgb) : Color<T, NUM_VALUES>(rgb.pigment) {}

  template <class T_, size_t NUM_VALUES_>
  constexpr RGB(const std::array<T_, NUM_VALUES_> &pigments)
      : Color<T, NUM_VALUES>(pigments) {}

  template <class T_>
  constexpr RGB(T_ red, T_ green, T_ blue) : Color<T, NUM_VALUES>(std::array<T_, 3>{{red, green, blue}}){}

  template <class T_>
  constexpr RGB(T_ red, T_ green, T_ blue, T_ alpha)
      : Color<T, NUM_VALUES>(std::array<T_, 4>{{red, green, blue, alpha}}) {}

  template <class T_, size_t NUM_VALUES_>
  requires color::NeedsConversation<T, T_, NUM_VALUES, NUM_VALUES_>
  constexpr RGB<T, NUM_VALUES> &operator=(const RGB<T_, NUM_VALUES_> &rgb) {
    if (&rgb == this) {
      return *this;
    }
    // constructor deals with different T
    const RGB<T, NUM_VALUES> temp(rgb);
    constexpr size_t min = std::min(NUM_VALUES_, NUM_VALUES);
    std::copy(&temp.pigment[0], &temp.pigment[0] + min, &this->pigment[0]);
    return *this;
  }

  constexpr T r() const { return this->pigment[0]; }
  constexpr T g() const { return this->pigment[1]; }
  constexpr T b() const { return this->pigment[2]; }

  constexpr T &r() { return this->pigment[0]; }
  constexpr T &g() { return this->pigment[1]; }
  constexpr T &b() { return this->pigment[2]; }

  std::string pigmentName(size_t i) const override {
    switch (i) {
      case 0U:
        return "R";
      case 1U:
        return "G";
      case 2U:
        return "B";
      case 3U:
        if (!has_alpha) {
          assert(("This color has no alpha value!", i == 0 || i == 1 || i == 2));
        }
        return "A";
      default:
        assert(("pigmentName only supports i in [0, 1, 2]", i == 0 || i == 1 || i == 2));
        return "?";
    }
  }

  std::string getColorTypeName() const override {
    if constexpr (has_alpha) {
      return "RGBA";
    }else{
      return "RGB";
    }
  }
};

template <class T, size_t NUM_VALUES=3>
class HSV : public Color<T, NUM_VALUES> {
 public:

  constexpr static bool has_alpha = (NUM_VALUES == 4);

  constexpr HSV() {}
  constexpr HSV(const HSV &hsv) = default;
  constexpr HSV(HSV &&hsv) = default;
  constexpr HSV &operator=(const HSV &hsv) = default;
  constexpr HSV &operator=(HSV &&hsv) = default;
  ~HSV() = default;

  template <class T_, size_t NUM_VALUES_>
  requires color::NeedsConversation<T, T_, NUM_VALUES, NUM_VALUES_>
  constexpr HSV(const HSV<T_, NUM_VALUES_> &hsv) : Color<T, NUM_VALUES>(hsv.pigment) {}

  template <class T_, size_t NUM_VALUES_>
  constexpr HSV(const std::array<T_, NUM_VALUES_> &pigments)
      : Color<T, NUM_VALUES>(pigments) {}

  template <class T_>
  constexpr HSV(T_ hue, T_ saturation, T_ value) : Color<T, NUM_VALUES>(std::array<T_, 3>{{hue, saturation, value}}) {}

  template <class T_>
  constexpr HSV(T_ hue, T_ saturation, T_ value, T_ alpha)
      : Color<T, NUM_VALUES>(std::array<T_, 4>{{hue, saturation, value, alpha}}) {}

  template <class T_, size_t NUM_VALUES_>
  requires color::NeedsConversation<T, T_, NUM_VALUES, NUM_VALUES_>
  constexpr HSV<T, NUM_VALUES> &operator=(const HSV<T, NUM_VALUES> &hsv) {
    if (&hsv == this) {
      return *this;
    }
    // constructor deals with different T
    const HSV<T, NUM_VALUES> temp(hsv);
    constexpr size_t min = std::min(NUM_VALUES_, NUM_VALUES);
    std::copy(&temp.pigment[0], &temp.pigment[0] + min, &this->pigment[0]);
    return *this;
  }

  std::string pigmentName(size_t i) const override {
    switch (i) {
      case 0U:
        return "H";
      case 1U:
        return "S";
      case 2U:
        return "V";
      case 3U:
        if (!has_alpha) {
          assert(("This color has no alpha value!", i == 0 || i == 1 || i == 2));
        }
        return "A";
      default:
        assert(("pigmentName only supports i in [0, 1, 2]", i == 0 || i == 1 || i == 2));
        return "?";
    }
  }

  constexpr T h() const { return this->pigment[0]; }
  constexpr T s() const { return this->pigment[1]; }
  constexpr T v() const { return this->pigment[2]; }

  constexpr T &h() { return this->pigment[0]; }
  constexpr T &s() { return this->pigment[1]; }
  constexpr T &v() { return this->pigment[2]; }


  std::string getColorTypeName() const override {
    if constexpr (has_alpha) {
      return "HSVA";
    }else{
      return "HSV";
    }
  }
};

// r[0-1], g[0-1], b[0-1] -> h[0-1], s[0-1], v[0-1]
template <size_t NUM_VALUES>
constexpr RGB<double, NUM_VALUES> convertToRGB(const HSV<double, NUM_VALUES> &hsv) {

  // https://www.rapidtables.com/convert/color/hsv-to-rgb.html

  const double h_grad = hsv.h() * 360.;
  const double C = hsv.v() * hsv.s();
  const double X = C * (1. - std::abs(std::fmod(h_grad / 60., 2.) - 1.));
  const double m = (hsv.v() - C);

  RGB<double, NUM_VALUES> rgb;
  if (h_grad < 60.) {
    rgb = RGB<double>(C + m, X + m, m);
  } else if (h_grad < 120.) {
    rgb = RGB<double>(X + m, C + m, m);
  } else if (h_grad < 180.) {
    rgb = RGB<double>(m, C + m, X + m);
  } else if (h_grad < 240.) {
    rgb = RGB<double>(m, X + m, C + m);
  } else if (h_grad < 300.) {
    rgb = RGB<double>(X + m, m, C + m);
  } else {
    rgb = RGB<double>(C + m, m, X + m);
  }
  if constexpr (NUM_VALUES == 4) {
    rgb.a() = hsv.a();
  }
  return rgb;
}

template <typename T, size_t NUM_VALUES>
constexpr RGB<double, NUM_VALUES> convertToRGB(const HSV<T, NUM_VALUES> &hsv){
  return convertToRGB(HSV<double, NUM_VALUES>(hsv));
}

// h[0-1], s[0-1], v[0-1] -> r[0-1], g[0-1], b[0-1]
template <size_t NUM_VALUES>
static HSV<double, NUM_VALUES> convertToHSV(const RGB<double, NUM_VALUES> &rgb) {
  HSV<double, NUM_VALUES> hsv;
  // https://www.rapidtables.com/convert/color/rgb-to-hsv.html

  const double Cmax = std::max(std::max(rgb.r(), rgb.g()), rgb.b());
  const double Cmin = std::min(std::min(rgb.r(), rgb.g()), rgb.b());
  const double delta = Cmax - Cmin;

  constexpr double SMALL_NUMBER = 0.00000001;

  if (delta < SMALL_NUMBER) {
    hsv.h() = 0;
  } else if (std::abs(rgb.r() - Cmax) < SMALL_NUMBER) {
    hsv.h() = 60. * std::fmod((rgb.g() - rgb.b()) / delta, 6.) / 360.;
  } else if (std::abs(rgb.g() - Cmax) < SMALL_NUMBER) {
    hsv.h() = 60. * (((rgb.b() - rgb.r()) / delta) + 2.) / 360.;
  } else {
    hsv.h() = 60. * (((rgb.r() - rgb.g()) / delta) + 4.) / 360.;
  }

  if (hsv.h() < 0) {
    hsv.h() += 1.;
  }

  if (Cmax < SMALL_NUMBER) {
    hsv.s() = 0;
  } else {
    hsv.s() = delta / Cmax;
  }

  hsv.v() = Cmax;

  if constexpr (NUM_VALUES == 4) {
    hsv.a() = rgb.a();
  }
  return hsv;
}

template <typename T, size_t NUM_VALUES>
static HSV<double, NUM_VALUES> convertToHSV(const RGB<T, NUM_VALUES> &rgb){
  return convertToHSV(RGB<double, NUM_VALUES>(rgb));
}

}  // namespace color

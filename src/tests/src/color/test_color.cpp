/**
 * @file test_color.cpp
 * @brief Unit Tests using Catch2 for the color::HSV and color::RGB class
 * @date 30.08.2025
 * @author Jakob Wandel
 * @version 1.0
 **/

#include <array>
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <color/color.hpp>
#include <cstdlib>
#include <iostream>
#include <ostream>

TEST_CASE("color_test_float_int_conversation") {
  constexpr double TOLERANCE = 0.00001;

  constexpr int MAX_INTENSITY = 255;
  for (int byteValue = 0; byteValue < MAX_INTENSITY; ++byteValue) {
    const std::array<int, 4> pigments_i = {{byteValue, byteValue, byteValue, byteValue}};

    const color::HSV<int, 4> hsvi(pigments_i);
    const color::RGB<int, 4> rgbi(pigments_i);

    const color::HSV<double, 4> hsvd(hsvi);
    const color::RGB<double, 4> rgbd(rgbi);

    const color::HSV<int, 4> hsvi2(hsvd);
    const color::RGB<int, 4> rgbi2(rgbd);

    const color::HSV<double, 4> hsvd2(hsvi2);
    const color::RGB<double, 4> rgbd2(rgbi2);

    REQUIRE(hsvi.h() == hsvi2.h());
    REQUIRE(hsvi.s() == hsvi2.s());
    REQUIRE(hsvi.v() == hsvi2.v());
    REQUIRE(hsvi.a() == hsvi2.a());

    REQUIRE(rgbi.r() == rgbi2.r());
    REQUIRE(rgbi.g() == rgbi2.g());
    REQUIRE(rgbi.b() == rgbi2.b());
    REQUIRE(rgbi.a() == rgbi2.a());

    REQUIRE(hsvd.h() == Catch::Approx(hsvd2.h()).epsilon(TOLERANCE));
    REQUIRE(hsvd.s() == Catch::Approx(hsvd2.s()).epsilon(TOLERANCE));
    REQUIRE(hsvd.v() == Catch::Approx(hsvd2.v()).epsilon(TOLERANCE));
    REQUIRE(hsvd.a() == Catch::Approx(hsvd2.a()).epsilon(TOLERANCE));

    REQUIRE(rgbd.r() == Catch::Approx(rgbd2.r()).epsilon(TOLERANCE));
    REQUIRE(rgbd.g() == Catch::Approx(rgbd2.g()).epsilon(TOLERANCE));
    REQUIRE(rgbd.b() == Catch::Approx(rgbd2.b()).epsilon(TOLERANCE));
    REQUIRE(rgbd.a() == Catch::Approx(rgbd2.a()).epsilon(TOLERANCE));
  }
}

TEST_CASE("color_init_wrong_type") {
  constexpr double TOLERANCE          = 0.00001;
  const std::array<int, 4> pigments_i = {
    {200, 200, 200, 200}};  // NOLINT (readability-magic-numbers) // yes these are random numbers without meaning

  double const result_expected =
    200. / 255.;  // NOLINT (readability-magic-numbers) // yes these are random numbers without meaning

  const color::HSV<double, 4> hsvd(pigments_i);
  const color::RGB<double, 4> rgbd(pigments_i);

  REQUIRE(hsvd.h() == Catch::Approx(result_expected).epsilon(TOLERANCE));
  REQUIRE(hsvd.s() == Catch::Approx(result_expected).epsilon(TOLERANCE));
  REQUIRE(hsvd.v() == Catch::Approx(result_expected).epsilon(TOLERANCE));
  REQUIRE(hsvd.a() == Catch::Approx(result_expected).epsilon(TOLERANCE));

  REQUIRE(rgbd.r() == Catch::Approx(result_expected).epsilon(TOLERANCE));
  REQUIRE(rgbd.g() == Catch::Approx(result_expected).epsilon(TOLERANCE));
  REQUIRE(rgbd.b() == Catch::Approx(result_expected).epsilon(TOLERANCE));
  REQUIRE(rgbd.a() == Catch::Approx(result_expected).epsilon(TOLERANCE));
}

TEST_CASE("color_test_hsv_rgb_conversation") {
  constexpr double TOLERANCE = 0.0001;

  constexpr int MAX_INTENSITY = 255;

  for (int byteValue = 0; byteValue < MAX_INTENSITY; ++byteValue) {
    const double normalyzedByteValue =
      static_cast<double>(byteValue) / static_cast<double>(MAX_INTENSITY);
    const std::array<int, 3> pigments_i = {{byteValue, byteValue, byteValue}};
    const std::array<double, 3> pigments_d = {
      {normalyzedByteValue, normalyzedByteValue, normalyzedByteValue}};

    const color::HSV<int> hsvi(pigments_i);
    const color::HSV<double> hsvid(hsvi);
    const color::RGB<double> rgbdi(color::convertToRGB(hsvid));
    const color::HSV<int> hsvid2(color::convertToHSV(rgbdi));

    REQUIRE(std::abs(hsvi.h() - hsvid2.h()) <= 1);
    REQUIRE(std::abs(hsvi.s() - hsvid2.s()) <= 1);
    REQUIRE(std::abs(hsvi.v() - hsvid2.v()) <= 1);

    const color::RGB<int> rgbi(pigments_i);
    const color::RGB<double> rgbdi2(rgbi);
    const color::HSV<double> hsvdi2(color::convertToHSV(rgbdi2));
    const color::RGB<int> rgbdi22(color::convertToRGB(hsvdi2));

    REQUIRE(std::abs(rgbi.r() - rgbdi22.r()) <= 1);
    REQUIRE(std::abs(rgbi.g() - rgbdi22.g()) <= 1);
    REQUIRE(std::abs(rgbi.b() - rgbdi22.b()) <= 1);

    const color::HSV<double> hsvd(pigments_d);
    const color::RGB<double> rgbd(color::convertToRGB(hsvd));
    const color::HSV<double> hsvd2(color::convertToHSV(rgbd));
    const color::RGB<double> rgbd2(color::convertToRGB(hsvd2));

    // conversion double: hsv->rgb->hsv->rgb
    REQUIRE(hsvd.h() == Catch::Approx(hsvd2.h()).epsilon(TOLERANCE));
    REQUIRE(hsvd.s() == Catch::Approx(hsvd2.s()).epsilon(TOLERANCE));
    REQUIRE(hsvd.v() == Catch::Approx(hsvd2.v()).epsilon(TOLERANCE));
    REQUIRE(rgbd.r() == Catch::Approx(rgbd2.r()).epsilon(TOLERANCE));
    REQUIRE(rgbd.g() == Catch::Approx(rgbd2.g()).epsilon(TOLERANCE));
    REQUIRE(rgbd.b() == Catch::Approx(rgbd2.b()).epsilon(TOLERANCE));
  }
}

TEST_CASE("color_test_rgba") {
  constexpr double TOLERANCE = 0.0001;

  // NOLINTBEGIN(readability-magic-numbers) // yes these are random numbers without meaning
  const std::array<int, 4> pigments_i = {{34, 44, 67, 50}};

  const color::RGB<int, 4> rgbai(pigments_i);
  const color::HSV<int, 4> hsvai(pigments_i);

  const color::RGB<double, 4> rgbad(pigments_i);
  const color::HSV<double, 4> hsvad(pigments_i);

  // this shall not throw
  REQUIRE_NOTHROW(std::cout << rgbai.a() << '\n');
  REQUIRE_NOTHROW(std::cout << hsvai.a() << '\n');
  REQUIRE_NOTHROW(std::cout << rgbad.a() << '\n');
  REQUIRE_NOTHROW(std::cout << hsvad.a() << '\n');

  const color::RGB<int, 4> rgbai2(pigments_i);
  const color::HSV<int, 4> hsvai2(pigments_i);

  const color::RGB<double, 4> rgbad2(pigments_i);
  const color::HSV<double, 4> hsvad2(pigments_i);

  REQUIRE(rgbai2.a() == 50);  // copied from pigments_i
  REQUIRE(hsvai2.a() == 50);

  REQUIRE(rgbad2.a() == Catch::Approx(0.1960784314).epsilon(TOLERANCE));
  REQUIRE(hsvad2.a() == Catch::Approx(0.1960784314).epsilon(TOLERANCE));

  const color::HSV<double, 4> hsvd;

  REQUIRE_NOTHROW(std::cout << rgbai.a() << '\n');
  REQUIRE_NOTHROW(std::cout << hsvd << '\n');

  // default alpha is 100%
  REQUIRE(rgbai2.a() == 50);  // copied from pigments_i
  REQUIRE(hsvai2.a() == 50);

  // default alpha is 100%
  REQUIRE(hsvd.a() == Catch::Approx(1.).epsilon(TOLERANCE));
  // NOLINTEND(readability-magic-numbers)
}

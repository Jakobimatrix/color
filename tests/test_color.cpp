#define BOOST_TEST_MODULE color_test TestSuites
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <cmath>
#include <color.hpp>

namespace utf = boost::unit_test;
namespace tt = boost::test_tools;


BOOST_AUTO_TEST_CASE(color_test_float_int_conversation) {
  constexpr double TOLERANCE = 0.00001;

  for (int i = 0; i < 255; i++) {
    const std::array<int, 3> pigments_i = {{i, i, i}};

    const color::HSV<int> hsvi(pigments_i);
    const color::RGB<int> rgbi(pigments_i);

    const color::HSV<double> hsvd(hsvi);
    const color::RGB<double> rgbd(rgbi);

    const color::HSV<int> hsvi2(hsvd);
    const color::RGB<int> rgbi2(rgbd);

    const color::HSV<double> hsvd2(hsvi2);
    const color::RGB<double> rgbd2(rgbi2);

    BOOST_TEST(hsvi.h == hsvi2.h);
    BOOST_TEST(hsvi.s == hsvi2.s);
    BOOST_TEST(hsvi.v == hsvi2.v);

    BOOST_TEST(rgbi.r == rgbi2.r);
    BOOST_TEST(rgbi.g == rgbi2.g);
    BOOST_TEST(rgbi.b == rgbi2.b);

    BOOST_TEST(hsvd.h == hsvd2.h, tt::tolerance(TOLERANCE));
    BOOST_TEST(hsvd.s == hsvd2.s, tt::tolerance(TOLERANCE));
    BOOST_TEST(hsvd.v == hsvd2.v, tt::tolerance(TOLERANCE));

    BOOST_TEST(rgbd.r == rgbd2.r, tt::tolerance(TOLERANCE));
    BOOST_TEST(rgbd.g == rgbd2.g, tt::tolerance(TOLERANCE));
    BOOST_TEST(rgbd.b == rgbd2.b, tt::tolerance(TOLERANCE));
  }
}

BOOST_AUTO_TEST_CASE(color_test_hsv_rgb_conversation) {
  constexpr double TOLERANCE = 0.00001;

  // Converting between different color spaces using Integer wont result in the
  // exact same color and is a bad idea anyways. If you need to convert between
  // colors always use floating precision and only convert for drawing.
  constexpr double TOLERANCE_bad_f_i_f = 0.15;
  constexpr double TOLERANCE_bad_i_i_i = 20.00;

  for (int i = 0; i < 255; i++) {
    double d = static_cast<double>(i) / 255.;
    const std::array<int, 3> pigments_i = {{i, i, i}};
    const std::array<double, 3> pigments_d = {{d, d, d}};

    const color::HSV<int> hsvi(pigments_i);
    const color::RGB<int> rgbi(hsvi);
    const color::HSV<int> hsvi2(rgbi);

    const color::HSV<double> hsvd(pigments_d);
    const color::RGB<double> rgbd(hsvd);
    const color::HSV<double> hsvd2(rgbd);

    const color::HSV<int> hsvi_(pigments_i);
    const color::RGB<double> rgbd_(hsvi_);
    const color::HSV<int> hsvi2_(rgbd_);

    const color::HSV<double> hsvd_(pigments_d);
    const color::RGB<int> rgbi_(hsvd_);
    const color::HSV<double> hsvd2_(rgbi_);

    BOOST_TEST((double)hsvi.h == (double)hsvi2.h, tt::tolerance(TOLERANCE_bad_i_i_i));
    BOOST_TEST((double)hsvi.s == (double)hsvi2.s, tt::tolerance(TOLERANCE_bad_i_i_i));
    BOOST_TEST((double)hsvi.v == (double)hsvi2.v, tt::tolerance(TOLERANCE_bad_i_i_i));

    BOOST_TEST(hsvd.h == hsvd2.h, tt::tolerance(TOLERANCE));
    BOOST_TEST(hsvd.s == hsvd2.s, tt::tolerance(TOLERANCE));
    BOOST_TEST(hsvd.v == hsvd2.v, tt::tolerance(TOLERANCE));

    BOOST_TEST(hsvi_.h == hsvi2_.h);
    BOOST_TEST(hsvi_.s == hsvi2_.s);
    BOOST_TEST(hsvi_.v == hsvi2_.v);

    BOOST_TEST(std::abs(hsvd_.h - hsvd2_.h) < TOLERANCE_bad_f_i_f);
    BOOST_TEST(std::abs(hsvd_.s - hsvd2_.s) < TOLERANCE_bad_f_i_f);
    BOOST_TEST(std::abs(hsvd_.v - hsvd2_.v) < TOLERANCE_bad_f_i_f);
  }
}

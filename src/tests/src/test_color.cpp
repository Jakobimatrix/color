#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <color/color.hpp>

TEST_CASE("color_test_float_int_conversation") {
    constexpr double TOLERANCE = 0.00001;

    for (int i = 0; i < 255; i++) {
        const std::array<int, 4> pigments_i = {{i, i, i, i}};

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
    constexpr double TOLERANCE = 0.00001;
    const std::array<int, 4> pigments_i = {{200, 200, 200, 200}};

    double result_expected = 200. / 255.;

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
    constexpr double TOLERANCE = 0.00001;
    constexpr double TOLERANCE_bad_f_i_f = 0.15;
    constexpr double TOLERANCE_bad_i_i_i = 20.00;

    for (int i = 0; i < 255; i++) {
        double d = static_cast<double>(i) / 255.;
        const std::array<int, 3> pigments_i = {{i, i, i}};
        const std::array<double, 3> pigments_d = {{d, d, d}};

        const color::HSV<int> hsvi(pigments_i);
        const color::RGB<int> rgbi(color::convertToRGB(hsvi));
        const color::HSV<int> hsvi2(color::convertToHSV(rgbi));

        const color::HSV<double> hsvd(pigments_d);
        const color::RGB<double> rgbd(olor::convertToRGB(hsvd));
        const color::HSV<double> hsvd2(color::convertToHSV(rgbd));

        const color::HSV<int> hsvi_(pigments_i);
        const color::RGB<double> rgbd_(color::convertToRGB(hsvi_));
        const color::HSV<int> hsvi2_(color::convertToHSV(rgbd_));

        const color::HSV<double> hsvd_(pigments_d);
        const color::RGB<int> rgbi_(color::convertToRGB(hsvd_));
        const color::HSV<double> hsvd2_(color::convertToHSV(rgbi_));

        REQUIRE(static_cast<double>(hsvi.h) == Catch::Approx(static_cast<double>(hsvi2.h)).epsilon(TOLERANCE_bad_i_i_i));
        REQUIRE(static_cast<double>(hsvi.s) == Catch::Approx(static_cast<double>(hsvi2.s)).epsilon(TOLERANCE_bad_i_i_i));
        REQUIRE(static_cast<double>(hsvi.v) == Catch::Approx(static_cast<double>(hsvi2.v)).epsilon(TOLERANCE_bad_i_i_i));

        REQUIRE(hsvd.h() == Catch::Approx(hsvd2.h).epsilon(TOLERANCE));
        REQUIRE(hsvd.s() == Catch::Approx(hsvd2.s).epsilon(TOLERANCE));
        REQUIRE(hsvd.v() == Catch::Approx(hsvd2.v).epsilon(TOLERANCE));

        REQUIRE(hsvi_.h() == hsvi2_.h());
        REQUIRE(hsvi_.s() == hsvi2_.s());
        REQUIRE(hsvi_.v() == hsvi2_.v());

        REQUIRE(std::abs(hsvd_.h() - hsvd2_.h()) < TOLERANCE_bad_f_i_f);
        REQUIRE(std::abs(hsvd_.s() - hsvd2_.s()) < TOLERANCE_bad_f_i_f);
        REQUIRE(std::abs(hsvd_.v() - hsvd2_.v()) < TOLERANCE_bad_f_i_f);
    }
}

TEST_CASE("color_test_rgba") {
    constexpr double TOLERANCE = 0.0000000001;

    const std::array<int, 4> pigments_i = {{34, 44, 67, 50}};

    const color::RGB<int, 4> rgbai(pigments_i);
    const color::HSV<int, 4> hsvai(pigments_i);

    const color::RGB<double, 4> rgbad(pigments_i);
    const color::HSV<double, 4> hsvad(pigments_i);

    // this shall not throw
    REQUIRE_NOTHROW(std::cout << rgbai.a << std::endl);
    REQUIRE_NOTHROW(std::cout << hsvai.a << std::endl);
    REQUIRE_NOTHROW(std::cout << rgbad.a << std::endl);
    REQUIRE_NOTHROW(std::cout << hsvad.a << std::endl);

    const std::array<int, 3> pigments_i2 = {{34, 44, 67}};

    const color::RGB<int, 4> rgbai2(pigments_i);
    const color::HSV<int, 4> hsvai2(pigments_i);

    const color::RGB<double, 4> rgbad2(pigments_i);
    const color::HSV<double, 4> hsvad2(pigments_i);

    // default alpha is 100%
    REQUIRE(rgbai2.a() == 255);
    REQUIRE(hsvai2.a() == 255);

    // default alpha is 100%
    REQUIRE(rgbad2.a() == Catch::Approx(1.).epsilon(TOLERANCE));
    REQUIRE(hsvad2.a() == Catch::Approx(1.).epsilon(TOLERANCE));

    const color::RGB<double> rgbd(pigments_i2);
    const color::HSV<double> hsvd(pigments_i2);

    // The following shall not throw
    REQUIRE_NOTHROW(const color::RGB<int, 4> rgbai3(rgbd));
    REQUIRE_NOTHROW(color::HSV<double, 3> hsvd3(hsvai));

    REQUIRE_NOTHROW(std::cout << rgbai.a() << std::endl);
    REQUIRE_NOTHROW(std::cout << hsvd << std::endl);
}


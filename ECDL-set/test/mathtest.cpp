#include <gtest/gtest.h>
#include "../include/MathUtil.hpp"
#include "../include/EllipricCurve.hpp"

#include <boost/multiprecision/cpp_int.hpp>
using namespace boost::multiprecision;
using namespace boost::multiprecision::literals;


TEST(_Math_Utils, __pow_test)
{
    ASSERT_EQ( Util::Math<int2048_t>::mod(50,16), 2);
}

TEST(_Math_Utils, __pow_test_inverted)
{
    ASSERT_EQ( Util::Math<int2048_t>::mod(-50,16), 14);
}

TEST(_Math_Utils, __pow_finite)
{
    ASSERT_EQ( Util::Math<int2048_t>::pow_finite(13,14,15), 4);
}

TEST(_Math_Utils, __invert)
{
    ASSERT_EQ( Util::Math<int2048_t>::invert(7,15), 13);
}

TEST(_ECC_EC_points, __mul_simple_1)
{
    ECC::EllipticPoint<int2048_t> a(10,8,23,0);

    ECC::EllipticPoint<int2048_t> res(22,12,23,0);
    ECC::EllipticPoint<int2048_t> check = 5*a; 
    ASSERT_EQ( check , res);
}

TEST(_ECC_EC_points, __mul_simple_2)
{
    ECC::EllipticPoint<int2048_t> b(22,12,23,0);

    ECC::EllipticPoint<int2048_t> res(22,11,23,0);

    ASSERT_EQ( (23*b) , res);
}

TEST(_ECC_EC_points, __sum_simple)
{
    ECC::EllipticPoint<int2048_t> a(22,12,23,0);

    ECC::EllipticPoint<int2048_t> b(10,8,23,0);

    ECC::EllipticPoint<int2048_t> res(9,0,23,0);

    ASSERT_EQ(a+b, res);
}

TEST(_ECC_EC_points, __sum_complex)
{
    ECC::EllipticPoint<int2048_t> a(22,12,23,0);
    int2048_t m1 = 10;
    ECC::EllipticPoint<int2048_t> b(1,10,23,0);
    int2048_t m2 = 3;
    ECC::EllipticPoint<int2048_t> res(8,6,23,0);

    ASSERT_EQ( ((m1*a) + (m2*b)) , res);
}

TEST(_ECC_EC_points, __sum_to_inf)
{
    ECC::EllipticPoint<int2048_t> res1(22,11,23,0);

    ECC::EllipticPoint<int2048_t> res2(22,12,23,0);

    ASSERT_EQ(res1+res2, ECC::EllipticPoint<int2048_t>(true));
}

TEST(_ECC_EC_points, __sum_with_inf)
{
    ECC::EllipticPoint<int2048_t> res1(22,11,23,0);
    ECC::EllipticPoint<int2048_t> i(true);

    ASSERT_EQ(res1+i, res1);
}

TEST(_ECC_EC_curve, __on_curve_check)
{
    ECC::EllipticCurve<int2048_t> ec(0,7,23);
    ECC::EllipticPoint<int2048_t> a(15,1,23,0);

    ASSERT_EQ(ec.CheckPoint(a), true);
}
